/* packer for INNOCOMM firmware
 * Author: Franco Venturi
 * Header CRC32 checksum: chinabull
 * Image SHA1 checksum: chinabull
 * Version: 1.0 - Sun Jan 22 20:01:40 EST 2012
gcc -O -Wall -o packer packer.c -lz -lcrypto
gcc -O -Wall -o unpacker unpacker.c -lz -lcrypto

unpacker [-v] [-c] <firmware file>

-v verbose; prints a bunch of debugging information to stderr
-c print configuration; prints the configuration file for the 'packer' to stdout

unpacker -c griffin.ifw > griffin.conf

The configuration file is a simple text file and, if you want to start 'packing' your own firmware for the TC970/griffin, you may want to take a look at it (and possibly edit it to your liking before 'packing')

packer mygriffin.ifw < griffin.conf    */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <zlib.h>
#include <utime.h>
#include <openssl/sha.h>


/* some useful definitions */
enum { MaxImages = 13 };

static const u_int32_t TRAILER_MAGIC = 0xcafeefac;

/* 136 bytes */
typedef struct {
    char name[16];
    u_int32_t offset;
    u_int32_t comp_level;
    u_int32_t comp_size;
    u_int32_t uncomp_size;
    char version[104];
} image_index_t;

/* 2048 bytes */
typedef struct {
    char magic[10];
    u_int16_t header_version;
    u_int32_t time;
    u_int32_t header_checksum;
    unsigned char checksum[20];
    unsigned char zeros1[12];
    u_int32_t length;
    char name[16];
    char version[32];
    unsigned char zeros2[72];
    image_index_t imind[MaxImages];
    unsigned char zeros3[104];
} firmware_header_t;

/* 2048 bytes */
typedef struct {
    char magic[10];
    u_int16_t header_version;
    u_int32_t load_address;
    u_int32_t filesize_with_cafeefac;
    u_int32_t time;
    u_int32_t filesize;
    u_int32_t header_checksum;
    unsigned char sha1digest[20];
    unsigned char zeros1[92];
    char name[16];
    char version[32];
    unsigned char zeros2[1856];
} image_header_t;


/* some useful functions */
static void usage();
static int initialize_firmware_header(int fd, firmware_header_t *fwhea,
    char *name, char *version, off_t *start_of_header);
static int add_image_to_firmware(int fdfw, image_index_t *imind, char *name,
    char *version, u_int32_t comp_level, u_int32_t load_address);
static int make_image_header(image_header_t *imhea, int fd, char *name,
    char *version, u_int32_t load_address);
static int add_image_uncompressed(image_header_t *imhea, int fdin, int fdout,
    ssize_t *uncomp_size, ssize_t *comp_size);
static int add_image_compressed(image_header_t *imhea, int fdin, int fdout,
    u_int32_t comp_level, ssize_t *uncomp_size, ssize_t *comp_size);
static int finalize_firmware_header(int fd, firmware_header_t *fwhea,
    off_t *start_of_header);


int main(int argc, char **argv) {

    enum { LineLength = 256 };

    int fdfw, rc;
    char line[LineLength], name[LineLength], version[LineLength];
    char c1, c2;
    u_int32_t comp_level, load_address;
    firmware_header_t fwh;
    off_t start_of_fw_header = 0L;
    image_index_t *imind = NULL;


    if (argc != 2) {
        usage();
        return 1;
    }

    if ((fdfw = open(argv[1], O_WRONLY | O_CREAT, 0644)) == -1) {
        fprintf(stderr, "open(%s) for writing failed: %s\n", argv[1], strerror(errno));
        return 2;
    }

    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '#') continue;
        if (sscanf(line, "%s %s %c %c", name, version, &c1, &c2) == 4 &&
            c1 == '-' && c2 == '-') {
            if ((rc = initialize_firmware_header(fdfw, &fwh, name, version,
                               &start_of_fw_header)) !=0) {
                close(fdfw);
                return rc;
           }
           imind = fwh.imind;
        } else if (sscanf(line, "%s %s %d %i", name, version, &comp_level,
                        &load_address) == 4) {
            if ((rc = add_image_to_firmware(fdfw, imind, name, version,
                            comp_level, load_address)) != 0) {
                close(fdfw);
                return rc;
            }
            imind++;
        } else {
            fprintf(stderr, "invalid configuration line: %s\n", line);
            close(fdfw);
            return 3;
        }
    }

    if ((rc = finalize_firmware_header(fdfw, &fwh, &start_of_fw_header)) != 0) {
        close(fdfw);
        return rc;
    }

    close(fdfw);
    return 0;
}


static void usage()
{
    fprintf(stderr, "usage: packer <firmware>\n");
}


static int initialize_firmware_header(int fd, firmware_header_t *fwhea,
    char *name, char *version, off_t *start_of_header)
{
    ssize_t nwritten;


    memset(fwhea, 0, sizeof(firmware_header_t));
    strncpy(fwhea->magic, "#INNOCOMM#", sizeof(fwhea->magic));
    fwhea->header_version = 1;
    fwhea->time = time(0);
    strncpy(fwhea->name, name, sizeof(fwhea->name));
    strncpy(fwhea->version, version, sizeof(fwhea->version));

    if ((*start_of_header = lseek(fd, 0, SEEK_CUR)) == (off_t)-1) {
        fprintf(stderr, "tell() failed: %s\n", strerror(errno));
        return 11;
    }

    if ((nwritten = write(fd, fwhea, sizeof(firmware_header_t))) == -1) {
        fprintf(stderr, "write(firmware_header) failed: %s\n", strerror(errno));
        return 12;
    }
    if (nwritten != sizeof(firmware_header_t)) {
        fprintf(stderr, "incomplete write for firmware header\n");
        return 13;
    }

    return 0;
}


static int add_image_to_firmware(int fdfw, image_index_t *imind, char *name,
    char *version, u_int32_t comp_level, u_int32_t load_address)
{
    int fdim, rc;
    off_t start_of_image;
    image_header_t imhea;
    ssize_t uncomp_size, comp_size;


    if ((start_of_image = lseek(fdfw, 0, SEEK_CUR)) == (off_t)-1) {
        fprintf(stderr, "tell() failed: %s\n", strerror(errno));
        return 21;
    }

    if ((fdim = open(name, O_RDONLY)) == -1) {
        fprintf(stderr, "open(%s) failed: %s\n", name, strerror(errno));
        return 22;
    }

    if ((rc = make_image_header(&imhea, fdim, name, version, load_address)) != 0) {
        close(fdim);
        return rc;
    }

    /* write the image header + image + trailer to firmware file */
    if (comp_level == 0) {
        if ((rc = add_image_uncompressed(&imhea, fdim, fdfw, &uncomp_size,
                                         &comp_size)) != 0) {
            close(fdim);
            return rc;
        }
    } else {
        if ((rc = add_image_compressed(&imhea, fdim, fdfw, comp_level,
                                       &uncomp_size, &comp_size)) != 0) {
            close(fdim);
            return rc;
        }
    }
    close(fdim);

    /* last thing: update firmware header index */
    strncpy(imind->name, name, sizeof(imind->name));
    imind->offset = start_of_image;
    imind->comp_level = comp_level;
    imind->comp_size = comp_size;
    imind->uncomp_size = uncomp_size;
    strncpy(imind->version, version, sizeof(imind->version));

    fprintf(stderr, "Added %s (%d bytes)\n", name, imhea.filesize);

    return 0;
}


static int make_image_header(image_header_t *imhea, int fd, char *name,
    char *version, u_int32_t load_address)
{
    struct stat stat;
    ssize_t nleft, nread;
    u_int32_t filesize, filesize_with_cafeefac;
    SHA_CTX sha_ctx;
    unsigned char digest[SHA_DIGEST_LENGTH];
    enum { BufferSize = 65536 };
    unsigned char buffer[BufferSize];


    if (fstat(fd, &stat) == -1) {
        fprintf(stderr, "stat(%s) failed: %s\n", name, strerror(errno));
        return 31;
    }
    filesize = stat.st_size;
    filesize_with_cafeefac = filesize + sizeof(TRAILER_MAGIC);

    memset(imhea, 0, sizeof(image_header_t));
    strncpy(imhea->magic, "@INNOCOMM@", sizeof(imhea->magic));
    imhea->header_version = 1;
    imhea->load_address = load_address;
    imhea->filesize_with_cafeefac = filesize_with_cafeefac;
    imhea->time = stat.st_mtime;
    imhea->filesize = filesize;
    strncpy(imhea->name, name, sizeof(imhea->name));
    strncpy(imhea->version, version, sizeof(imhea->version));

    /* we need to compute the SHA1 digest to create the full image header */
    if (SHA1_Init(&sha_ctx) != 1) {
        fprintf(stderr, "SHA1_Init() failed\n");
        return 32;
    }
    for (nleft = filesize; nleft > 0; nleft -= nread) {
        if ((nread = read(fd, buffer, nleft < BufferSize ?
                          nleft : BufferSize)) == -1) {
            fprintf(stderr, "read(image) failed: %s\n", strerror(errno));
            return 33;
        }
        if (nread == 0) {
            fprintf(stderr, "premature EOF while reading image\n");
            return 34;
        }
        if (SHA1_Update(&sha_ctx, buffer, nread) != 1) {
            fprintf(stderr, "SHA1_Update() failed\n");
            return 35;
        }
    }

    if (lseek(fd, 0, SEEK_SET) == (off_t)-1) {
        fprintf(stderr, "lseek() failed: %s\n", strerror(errno));
        return 36;
    }

    if (SHA1_Update(&sha_ctx, &TRAILER_MAGIC, sizeof(TRAILER_MAGIC)) != 1) {
        fprintf(stderr, "SHA1_Update() failed\n");
        return 37;
    }
    if (SHA1_Update(&sha_ctx, &filesize_with_cafeefac, sizeof(filesize_with_cafeefac)) != 1) {
        fprintf(stderr, "SHA1_Update() failed\n");
        return 38;
    }

    if (SHA1_Final(digest, &sha_ctx) != 1) {
        fprintf(stderr, "SHA1_Final() failed\n");
        return 39;
    }

    memcpy(imhea->sha1digest, digest, sizeof(imhea->sha1digest));
    imhea->header_checksum = crc32(0L, (const Bytef *)imhea,
        sizeof(image_header_t));

    return 0;
}


static int add_image_uncompressed(image_header_t *imhea, int fdin, int fdout,
    ssize_t *uncomp_size, ssize_t *comp_size)
{
    ssize_t nleft, nout, nread, nwritten, ntrailer;
    enum { BufferSize = 65536, BlockSize = 2048 };
    unsigned char buffer[BufferSize];


    nout = 0;

    /* 1 - write image header */
    if ((nwritten = write(fdout, imhea, sizeof(image_header_t))) == -1) {
        fprintf(stderr, "write(firmware) failed: %s\n", strerror(errno));
        return 41;
    }
    if (nwritten != sizeof(image_header_t)) {
        fprintf(stderr, "incomplete write for firmware\n");
        return 42;
    }
    nout += nwritten;

    /* 2 - write actual image */
    for (nleft = imhea->filesize; nleft > 0; nleft -= nread) {
        if ((nread = read(fdin, buffer, nleft < BufferSize ?
                          nleft : BufferSize)) == -1) {
            fprintf(stderr, "read(image) failed: %s\n", strerror(errno));
            return 43;
        }
        if (nread == 0) {
            fprintf(stderr, "premature EOF while reading image\n");
            return 44;
        }

        if ((nwritten = write(fdout, buffer, nread)) == -1) {
            fprintf(stderr, "write(firmware) failed: %s\n", strerror(errno));
            return 45;
        }
        if (nwritten != nread) {
            fprintf(stderr, "incomplete write for firmware\n");
            return 46;
        }
        nout += nwritten;
    }

    /* 3 - write trailer */
    ntrailer = BlockSize + sizeof(TRAILER_MAGIC) - 1 -
               (imhea->filesize_with_cafeefac - 1) % BlockSize;
    memset(buffer, 0, ntrailer);
    memcpy(buffer, &TRAILER_MAGIC, sizeof(TRAILER_MAGIC));

    if ((nwritten = write(fdout, buffer, ntrailer)) == -1) {
        fprintf(stderr, "write(firmware) failed: %s\n", strerror(errno));
        return 47;
    }
    if (nwritten != ntrailer) {
        fprintf(stderr, "incomplete write for firmware\n");
        return 48;
    }
    nout += nwritten;

    *uncomp_size = sizeof(image_header_t) + imhea->filesize + ntrailer;
    *comp_size = nout;

    return 0;
}


static int add_image_compressed(image_header_t *imhea, int fdin, int fdout,
    u_int32_t comp_level, ssize_t *uncomp_size, ssize_t *comp_size)
{
    ssize_t nleft, nout, nread, ntowrite, nwritten, ntrailer;
    unsigned char *writefromhere;
    int status, firstbytes;
    z_stream strm;
    enum { BufferSize = 65536, BlockSize = 2048 };
    unsigned char inbuf[BufferSize], outbuf[BufferSize];


    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    if (deflateInit(&strm, comp_level) != Z_OK) {
        fprintf(stderr, "deflateInit() failed\n");
        return 51;
    }

    nout = 0;
    firstbytes = 1;

    /* 1 - write image header */
    strm.avail_in = sizeof(image_header_t);
    strm.next_in = (unsigned char *)imhea;
    do {
        strm.avail_out = BufferSize;
        strm.next_out = outbuf;
        if ((status = deflate(&strm, Z_NO_FLUSH)) != Z_OK) {
            fprintf(stderr, "deflate() failed: %d\n", status);
            (void)deflateEnd(&strm);
            return 52;
        }
        /* skip the very first two bytes */
        writefromhere = outbuf;
        ntowrite = BufferSize - strm.avail_out;
        if (firstbytes) {
            firstbytes = 0;
            writefromhere += 2;
            ntowrite -= 2;
        }
        if ((nwritten = write(fdout, writefromhere, ntowrite)) == -1) {
            fprintf(stderr, "write(firmware) failed: %s\n", strerror(errno));
            (void)deflateEnd(&strm);
            return 53;
        }
        if (nwritten != ntowrite) {
            fprintf(stderr, "incomplete write for firmware\n");
            (void)deflateEnd(&strm);
            return 54;
        }
        nout += nwritten;
    } while (strm.avail_out == 0);

    /* 2 - write actual image */
    for (nleft = imhea->filesize; nleft > 0; nleft -= nread) {
        if ((nread = read(fdin, inbuf, nleft < BufferSize ?
                          nleft : BufferSize)) == -1) {
            fprintf(stderr, "read(image) failed: %s\n", strerror(errno));
            return 55;
        }
        if (nread == 0) {
            fprintf(stderr, "premature EOF while reading image\n");
            return 56;
        }

        strm.avail_in = nread;
        strm.next_in = inbuf;

        do {
            strm.avail_out = BufferSize;
            strm.next_out = outbuf;
            if ((status = deflate(&strm, Z_NO_FLUSH)) != Z_OK) {
                fprintf(stderr, "deflate() failed: %d\n", status);
                (void)deflateEnd(&strm);
                return 57;
            }

            ntowrite = BufferSize - strm.avail_out;
            if ((nwritten = write(fdout, outbuf, ntowrite)) == -1) {
                fprintf(stderr, "write(firmware) failed: %s\n", strerror(errno));
                (void)deflateEnd(&strm);
                return 58;
            }
            if (nwritten != ntowrite) {
                fprintf(stderr, "incomplete write for firmware\n");
                (void)deflateEnd(&strm);
                return 59;
            }
            nout += nwritten;
        } while (strm.avail_out == 0);
    }

    /* 3 - write trailer */
    ntrailer = BlockSize + sizeof(TRAILER_MAGIC) - 1 -
               (imhea->filesize_with_cafeefac - 1) % BlockSize;
    memset(inbuf, 0, ntrailer);
    memcpy(inbuf, &TRAILER_MAGIC, sizeof(TRAILER_MAGIC));

    strm.avail_in = ntrailer;
    strm.next_in = inbuf;

    do {
        strm.avail_out = BufferSize;
        strm.next_out = outbuf;
        status = deflate(&strm, Z_FINISH);
        if (status != Z_OK && status != Z_STREAM_END) {
            fprintf(stderr, "deflate() failed: %d\n", status);
            (void)deflateEnd(&strm);
            return 60;
        }

        ntowrite = BufferSize - strm.avail_out;
        if ((nwritten = write(fdout, outbuf, ntowrite)) == -1) {
            fprintf(stderr, "write(firmware) failed: %s\n", strerror(errno));
            (void)deflateEnd(&strm);
            return 61;
        }
        if (nwritten != ntowrite) {
            fprintf(stderr, "incomplete write for firmware\n");
            (void)deflateEnd(&strm);
            return 62;
        }
        nout += nwritten;
    } while (strm.avail_out == 0);

    if (status != Z_STREAM_END) {
        fprintf(stderr, "deflate() failed: %d\n", status);
        (void)deflateEnd(&strm);
        return 63;
    }

    /* clean up */
    (void)deflateEnd(&strm);

    *uncomp_size = sizeof(image_header_t) + imhea->filesize + ntrailer;
    *comp_size = nout;

    return 0;
}


static int finalize_firmware_header(int fd, firmware_header_t *fwhea,
    off_t *start_of_header)
{
    off_t end_of_file;
    ssize_t nwritten;


    if ((end_of_file = lseek(fd, 0, SEEK_END)) == (off_t)-1) {
        fprintf(stderr, "tell() failed: %s\n", strerror(errno));
        return 51;
    }

    fwhea->length = end_of_file;

    /* don't know what to put for the whole file signature, but it doesn't
     * look like the x-loader cares
     */
    strncpy((char *)fwhea->checksum, "UNKNOWN VALUE HERE!!",
        sizeof(fwhea->checksum));
    fwhea->header_checksum = crc32(0L, (const Bytef *)fwhea,
        sizeof(firmware_header_t));

    if (lseek(fd, *start_of_header, SEEK_SET) == (off_t)-1) {
        fprintf(stderr, "lseek() failed: %s\n", strerror(errno));
        return 52;
    }

    if ((nwritten = write(fd, fwhea, sizeof(firmware_header_t))) == -1) {
        fprintf(stderr, "write(firmware_header) failed: %s\n", strerror(errno));
        return 53;
    }

    return 0;
}
