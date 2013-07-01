#include <common_imp.h>

typedef struct {
    u32 count;
    u32 state[624];
} SceKernelUtilsMt19937Context;

typedef struct
{
    u32 h[4];
    u32 pad;
    u16 usRemains;
    u16 usComputed;
    u64 ullTotalLen;
    u8 buf[64];
} SceKernelUtilsMd5Context;

typedef struct
{
    u32 h[5];
    u16 usRemains;
    u16 usComputed;
    u64 ullTotalLen;
    u8 buf[64];
} SceKernelUtilsSha1Context;

int sceKernelDcacheInvalidateRangeForUser(const void *p, u32 size)
{
    int ret;
    int oldK1 = pspShiftK1();
    if (!pspK1DynBufOk(p, size))
        ret = 0x800200D3;
    else
        ret = sceKernelDcacheInvalidateRange(p, size);
    pspSetK1(oldK1);
    return ret;
}

int UtilsForUser_157A383A(const void *p, u32 size)
{
    int ret;
    int oldK1 = pspShiftK1();
    if (!pspK1DynBufOk(p, size))
        ret = 0x800200D3;
    else
        ret = UtilsForKernel_157A383A(p, size);
    pspSetK1(oldK1);
    return ret;
}

int sceKernelDcachePurgeRangeForUser(const void *p, u32 size)
{
    int ret;
    int oldK1 = pspShiftK1();
    if (!pspK1DynBufOk(p, size))
        ret = 0x800200D3;
    else
        ret = sceKernelDcachePurgeRange(p, size);
    pspSetK1(oldK1);
    return ret;
}

int sceKernelIcacheInvalidateRangeForUser(const void *p, u32 size)
{
    int ret;
    int oldK1 = pspShiftK1();
    if (!pspK1DynBufOk(p, size))
        ret = 0x800200D3;
    else
        ret = sceKernelIcacheInvalidateRange(p, size);
    pspSetK1(oldK1);
    return ret;
}

int UtilsForUser_43C9A8DB(const void *p, u32 size)
{
    int ret;
    int oldK1 = pspShiftK1();
    if (!pspK1DynBufOk(p, size))
        ret = 0x800200D3;
    else
        ret = UtilsForKernel_43C9A8DB(p, size);
    pspSetK1(oldK1);
    return ret;
}

int sceKernelUtilsMd5BlockUpdate(SceKernelUtilsMd5Context *ctx, u8 *data, u32 size)
{
    int oldK1 = pspShiftK1();
    if (!pspK1PtrOk(ctx)
     || !pspK1DynBufOk(data, size)
     || ctx == NULL || data == NULL) {
        pspSetK1(oldK1);
        return 0x800200D3;
    }
    // ED60
    if (ctx->usComputed != 0)
    {
        pspSetK1(oldK1);
        // EE44
        return -1;
    }
    int remaining = ctx->usRemains;
    if (ctx->usRemains >= 64)
    {
        // EE34
        pspSetK1(oldK1);
        return 0x800201BC;
    }
    ctx->ullTotalLen += size;
    int numTotal = ctx->usRemains + size;
    // EDB0
    while (numTotal >= 64)
    {
        if (remaining == 0)
        {
            // EE1C
            data += 64;
            memcpy(ctx->buf, data, 64);
        }
        else
        {
            data += 64 - remaining;
            memcpy(ctx + remaining + 32, data, 64 - remaining);
            remaining = 0;
        }
        // EDD4
        numTotal -= 64;
        sub_1710(ctx, ctx->buf);
    }
    // EDEC
    if (numTotal != 0) {
        // EE04
        memcpy(ctx + remaining + 32, data, numTotal - remaining);
    }
    ctx->usRemains = numTotal;
    // EDF8
    pspSetK1(oldK1);
    return 0;
}

int sceKernelUtilsMd5BlockResult(SceKernelUtilsMd5Context *ctx, u8 *digest)
{
    char buf[64];
    int oldK1 = pspShiftK1();
    if (!pspK1PtrOk(ctx) || !pspK1PtrOk(digest)
     || ctx == NULL || digest == NULL) {
        pspSetK1(oldK1);
        return 0x800200D3;
    }
    // EED0
    if (ctx->usComputed == 0)
    {
        short remain = ctx->usRemains;
        if (remain >= 64)
        {
            // EFE4
            pspSetK1(oldK1);
            return 0x800201BC;
        }
        memset(buf, 0, 64);
        memcpy(buf, ctx->buf, remain);
        buf[remain] = 0x80;
        if (remain >= 56)
        {
            // EFC0
            sub_1710(ctx, buf);
            memset(buf, 0, 64);
        }
        // EF1C
        u64 num = ctx->ullTotalLen * 8;
        // EF38
        int i;
        for (i = 0; i < 8; i++) {
            // EF74
            buf[i + 56] = num >> (i * 8);
        }
        sub_1710(ctx, buf);
        ctx->usComputed = 1;
        memset(ctx->buf, 0, 64);
    }
    // EFA8
    memcpy(digest, ctx, 16);
    pspSetK1(oldK1);
    return 0;
}

int sceKernelUtilsMd5Digest(u8 *data, u32 size, u8 *digest)
{
    SceKernelUtilsMd5Context ctx;
    int oldK1 = pspShiftK1();
    if (!pspK1DynBufOk(data, size) || !pspK1PtrOk(digest))
    {
        // F04C
        pspSetK1(oldK1);
        return 0x800200D3;
    }
    // F068
    if (pspK1PtrOk(&ctx)) // ?!?
    {
        ctx.h[0] = 0x67452301;
        ctx.h[1] = 0xEFCDAB89;
        ctx.h[2] = 0x89BADCFE;
        ctx.h[3] = 0x10325476;
        ctx.ullTotalLen = 0;
        ctx.usRemains = 0;
        ctx.usComputed = 0;
    }
    // F0B8
    sceKernelUtilsMd5BlockUpdate(&ctx, data, size);
    sceKernelUtilsMd5BlockResult(&ctx, digest);
    pspSetK1(oldK1);
    return 0;
}

int sceKernelUtilsMd5BlockInit(SceKernelUtilsMd5Context *ctx)
{
    int oldK1 = pspShiftK1();
    if (!pspK1PtrOk(ctx) || ctx == NULL) {
        pspSetK1(oldK1);
        return 0x800200D3;
    }
    ctx->h[0] = 0x67452301;
    ctx->ullTotalLen = 0;
    ctx->h[1] = 0xEFCDAB89;
    ctx->h[2] = 0x98BADCFE;
    ctx->h[3] = 0x10325476;
    ctx->usRemains = 0;
    ctx->usComputed = 0;
    pspSetK1(oldK1);
    return 0;
}

int sceKernelUtilsSha1BlockUpdate(SceKernelUtilsSha1Context *ctx, u8 *data, u32 size)
{
    int buf[16];
    int oldK1 = pspShiftK1();
    if (!pspK1PtrOk(ctx) || !pspK1DynBufOk(data, size) || ctx == NULL || data == NULL)
    {
        // F1B8
        pspSetK1(oldK1);
        return 0x800200D3;
    }
    // F1E8
    if (ctx->usComputed != 0)
    {
        pspSetK1(oldK1);
        // F2F8
        return -1;
    }
    u16 remaining = ctx->usRemains;
    if (remaining >= 64)
    {
        // F2E8
        pspSetK1(oldK1);
        return 0x800201BC;
    }
    ctx->ullTotalLen += size;
    u16 total = remaining + size;
    // F230
    while (total >= 64)
    {
        if (remaining == 0)
        {
            // F2D4
            memcpy(buf, data, 64);
            data += 64;
        }
        else
        {
            memcpy(buf, ctx->buf, remaining);
            data += 64 - remaining;
            memcpy(buf + remaining, data, 64 - remaining);
            remaining = 0;
        }
        // F264
        // F26C
        int i;
        for (i = 0; i < 16; i++)
            buf[i] = WSBW(buf[i]);
        total -= 64;
        sub_1870(buf, ctx);
    }
    // F2A4
    if (total != 0) {
        // F2BC
        memcpy(&ctx->buf[remaining], data, total - remaining);
    }
    ctx->usRemains = total;
    // F2B0
    pspSetK1(oldK1);
    return 0;
}

int sceKernelUtilsSha1BlockResult(SceKernelUtilsSha1Context *ctx, u8 *digest)
{
    int buf[16];
    int oldK1 = pspShiftK1();
    if (!pspK1PtrOk(ctx) || !pspK1PtrOk(digest) || ctx == NULL || digest == NULL)
    {
        // F358
        pspSetK1(oldK1);
        return 0x800200D3;
    }
    // F384
    if (ctx->usComputed == 0)
    {
        int i;
        if (ctx->usRemains >= 64)
        {
            // F500
            pspSetK1(oldK1);
            return 0x800201BC;
        }
        memset(buf, 0, 64);
        memcpy(buf, ctx->buf, ctx->usRemains);
        buf[ctx->usRemains] = 0x80;
        if (ctx->usRemains >= 56)
        {
            // F3D8
            for (i = 0; i < 16; i++)
                buf[i] = WSBW(buf[i]);
            sub_1870(buf, ctx->h);
            memset(buf, 0, 64);
        }
        // F410
        for (i = 0; i < 8; i++)
            ((char*)buf)[i + 56] = ctx->ullTotalLen >> (56 - i * 8);
        // F480
        for (i = 0; i < 16; i++)
            buf[i] = WSBW(buf[i]);
        sub_1870(buf, ctx);
        // F4B0
        for (i = 0; i < 5; i++)
            ctx->h[i] = WSBW(ctx->h[i]);
        ctx->usComputed = 1;
        memset(ctx->buf, 0, 64);
    }
    // F4E8
    memcpy(digest, ctx->h, 20);
    pspSetK1(oldK1);
    return 0;
}

int sceKernelUtilsSha1Digest(u8 *data, u32 size, u8 *digest)
{
    int oldK1 = pspShiftK1();
    if (!pspK1PtrOk(digest) || !pspK1DynBufOk(data, size))
    {
        // F550
        pspSetK1(oldK1);
        return 0x800200D3;
    }
    sub_1A40(data, size, digest);
    pspSetK1(oldK1);
    return 0;
}

int sceKernelUtilsSha1BlockInit(SceKernelUtilsSha1Context *ctx)
{
    int oldK1 = pspShiftK1();
    if (!pspK1PtrOk(ctx) || ctx == NULL)
    {
        // F5F8
        pspSetK1(oldK1);
        return 0x800200D3;
    }
    ctx->ullTotalLen = 0;
    ctx->h[0] = 0x67452301;
    ctx->h[1] = 0xEFCDAB89;
    ctx->h[2] = 0x98BADCFE;
    ctx->h[3] = 0x10325476;
    ctx->h[4] = 0xC3D2E1F0;
    ctx->usRemains = 0;
    ctx->usComputed = 0;
    pspSetK1(oldK1);
    return 0;
}

int sceKernelUtilsMt19937Init(SceKernelUtilsMt19937Context *ctx, u32 seed)
{
    int oldK1 = pspShiftK1();
    if (!pspK1PtrOk(ctx))
    {
        // F6A8
        pspSetK1(oldK1);
        return 0x800200D3;
    }
    ctx->state[0] = seed;
    // F640
    int i;
    for (i = 1; i < 624; i++)
        ctx->state[i] = (ctx->state[i - 1] ^ (ctx->state[i - 1] >> 30)) * 0x6C078965 + 1;
    ctx->count = 0;
    // F678
    for (i = 0; i < 624; i++)
        sceKernelUtilsMt19937UInt(ctx);
    pspSetK1(oldK1);
    return 0;
}

u32 sceKernelUtilsMt19937UInt(SceKernelUtilsMt19937Context *ctx)
{
    int ret;
    int oldK1 = pspShiftK1();
    if (!pspK1PtrOk(ctx))
        ret = 0x800200D3;
    else
        ret = sub_1CF0(ctx);
    pspSetK1(oldK1);
    return ret;
}

typedef clock_t (*ClockHandler)(void);
typedef time_t (*TimeHandler)(time_t *t);
typedef int (*GettimeofdayHandler)(struct timeval *tp, struct timezone *tzp);
typedef int (*RtcTickHandler)(u64 *tick);

// 14400
ClockHandler g_clock;
// 14404
TimeHandler g_time;
// 14408
GettimeofdayHandler g_gettimeofday;
// 1440C
RtcTickHandler g_rtctick;

int sceKernelRegisterRtcFunc(ClockHandler clock, TimeHandler time, GettimeofdayHandler gettimeofday, RtcTickHandler rtctick)
{
    g_clock = clock;
    g_time = time;
    g_gettimeofday = gettimeofday;
    g_rtctick = rtctick;
    return 0;
}

int sceKernelReleaseRtcFunc(void)
{
    g_clock = NULL;
    g_time = NULL;
    g_gettimeofday = NULL;
    g_rtctick = NULL;
    return 0;
}

clock_t sceKernelLibcClock(void)
{
    if (g_clock == NULL)
        return 0;
    int oldK1 = pspShiftK1();
    clock_t ret = g_clock();
    pspSetK1(oldK1);
    return ret;
}

time_t sceKernelLibcTime(time_t *t)
{
    time_t ret;
    if (g_time == NULL)
        return 0;
    int oldK1 = pspShiftK1();
    if (!pspK1PtrOk(t))
        ret = 0;
    else
        ret = g_time(t);
    pspSetK1(oldK1);
    return ret;
}

int sceKernelLibcGettimeofday(struct timeval *tp, struct timezone *tzp)
{
    int ret;
    if (g_gettimeofday == NULL)
        return 1;
    int oldK1 = pspShiftK1();
    if (!pspK1PtrOk(tp) || !pspK1PtrOk(tzp))
        ret = 1;
    else
        ret = g_gettimeofday(tp, tzp);
    pspSetK1(oldK1);
    return ret;
}

int sceKernelRtcGetTick(u64 *tick)
{
    if (g_rtctick == NULL)
        return 0x80000001;
    return g_rtctick(tick);
}

int sceKernelGzipDecompress(u8 *dest, u32 destSize, const u8 *src, u32 *unk)
{
    void *buf = sceKernelGzipGetCompressedData(src);
    void *unkPtr;
    int unkBuf[4];
    if (buf == NULL)
        return 0x80020001;
    if (src[2] != 8)
        return 0x80000004;
    int ret = sceKernelDeflateDecompress(dest, destSize, buf, &unkPtr);
    if (ret < 0)
        return ret;
    memcpy(unkBuf, unkPtr, 8);
    if (ret != unkBuf[1])
        return 0x80000108;
    if (unk != NULL)
        *unk = unkBuf[0];
    return ret;
}

int sceKernelGzipGetInfo(char *buf, char **extra, char **name, char **comment, short *crc16, char **compressedData)
{
    char *curBuf = buf + 10;
    u8 header[10];
    if (extra != NULL)
        *extra = NULL;
    if (name != NULL)
        *name = NULL;
    if (comment != NULL)
        *comment = NULL;
    if (crc16 != NULL)
        *crc16 = 0;
    if (compressedData != NULL)
        *compressedData = NULL;
    // F9AC
    if (buf == NULL)
        return 0x80000103;
    memcpy(header, buf, 10);
    if (header[0] != 0x1F || header[1] != 0x8B)
        return 0x80000108;
    // FA14
    char flag = header[3];
    if ((flag & 4) != 0) // FEXTRA
    {
        if (extra != NULL)
            *extra = curBuf;
        // FA38
        curBuf += (curBuf[1] << 8) | curBuf[0];
    }
    // FA40
    if ((flag & 8) != 0) // FNAME
    {
        if (name != NULL)
            *name = curBuf;
        // FA50
        // FA5C
        while (*curBuf != 0)
            curBuf++;
        // FA68
    }
    // FA6C
    if ((flag & 0x10) != 0) // FCOMMENT
    {
        if (comment != NULL)
            *comment = curBuf;
        // FA7C
        // FA88
        while (*curBuf != 0)
            curBuf++;
        // FA94
    }
    // FA98
    if ((flag & 2) != 0) // FHCRC
    {
        if (crc16 != NULL)
            *crc16 = (curBuf[1] << 8) | curBuf[0];
        curBuf += 2;
    }
    // FAC0
    if (compressedData != NULL)
        *compressedData = curBuf;
    // FAC8
    return 0;
}

char *sceKernelGzipGetCompressedData(char *buf)
{
    char *data;
    if (sceKernelGzipGetInfo(buf, NULL, NULL, NULL, NULL, &data) < 0)
        return NULL;
    return data;
}

int sceKernelGzipIsValid(u8 *buf)
{
    if (buf[0] == 0x1F && buf[1] == 0x8B)
        return 1;
    return 0;
}

char *sceKernelGzipGetName(char *buf)
{
    char *name;
    if (sceKernelGzipGetInfo(buf, NULL, &name, NULL, NULL, NULL) < 0)
        return NULL;
    return name;
}

char *sceKernelGzipGetComment(char *buf)
{
    char *comment;
    if (sceKernelGzipGetInfo(buf, NULL, NULL, &comment, NULL, NULL) < 0)
        return NULL;
    return comment;
}

