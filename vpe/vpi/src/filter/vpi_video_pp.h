/*
 * Copyright 2019 VeriSilicon, Inc.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef VPI_VIDEO_PP_H
#define VPI_VIDEO_PP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "vpi_types.h"
#include "ppapi.h"
#include "decapicommon.h"
#include "trans_edma_api.h"
#include "fifo.h"

#ifdef SUPPORT_TCACHE
#include "fb_ips.h"
#ifdef DRV_NEW_ARCH
#include "transcoder.h"
#else
#include "trans_edma.h"
#endif
#include "tcache_api.h"
#endif

typedef const void *pp_raw_parser_inst;
#define OUTPUT_BUF_DEPTH 68 /* support VCE lookaheadDepth 40 */

typedef struct {
    u32 in_width;
    u32 in_height;
    u32 in_width_align;
    u32 in_stride;
    u32 in_height_align;
    u32 align;
    u32 max_num_pics;
    u32 frame_size;
    int in_p010;
    int compress_bypass;
    int cache_enable;
    int shaper_enable;
    int pp_enabled;
    int out_p010;
#ifdef SUPPORT_TCACHE
    u32 crop_enabled;
    u32 crop_w;
    u32 crop_h;
#endif
} VpiPPConfig;

#ifdef SUPPORT_TCACHE
typedef struct {
    TCACHE_PIX_FMT t_in_fmt;
    TCACHE_PIX_FMT t_out_fmt;
    TCACHE_COV_BD rgb_cov_bd;

    u32 t_in_width;
    u32 t_in_height;
    u32 t_wplanes;
    u32 t_in_stride[MAX_INPUT_PLANE];
    u32 t_in_align_stride[MAX_INPUT_PLANE];
    u32 t_in_plane_height[MAX_INPUT_PLANE];
    u32 t_in_plane_size[MAX_INPUT_PLANE];

    u32 t_out_width;
    u32 t_out_height;
    u32 t_rplanes;
    u32 t_out_stride[MAX_OUTPUT_PLANE];
    u32 t_out_align_stride[MAX_OUTPUT_PLANE];
    u32 t_out_plane_height[MAX_OUTPUT_PLANE];
    u32 t_out_plane_size[MAX_OUTPUT_PLANE];
} TcacheContext;
#endif

enum StreamReadMode {
    STREAMREADMODE_FRAME      = 0,
    STREAMREADMODE_NALUNIT    = 1,
    STREAMREADMODE_FULLSTREAM = 2,
    STREAMREADMODE_PACKETIZE  = 3
};

typedef struct VpiRawParser {
    FILE *file;
    off_t file_size;
    u32 img_width;
    u32 img_height;
    VpiPixsFmt format;
    u32 planes;
    u32 byte_width[3];
    u32 stride[3];
    u32 height[3];
    u32 height_align[3];
    u32 plane_size[3];
    u32 frame_size;

    u32 hugepage_frame_size;
    u32 entry_num;
} VpiRawParser;

typedef struct VpiPPParams {
    char *in_file_name;
    char *out_file_name[2 * DEC_MAX_OUT_COUNT];
    u32 num_of_decoded_pics;
    enum DecPictureFormat format;
    enum DecPictureFormat hw_format;
    u32 pp_enabled;
    u8 display_cropped;
    u8 hw_traces;
    u8 trace_target;
    u8 extra_output_thread;
    u8 disable_display_order;
    enum StreamReadMode read_mode;
    enum DecErrorConcealment concealment_mode;
    enum DecDecoderMode decoder_mode;
    struct DecFixedScaleCfg fscale_cfg;
    PpUnitConfig ppu_cfg[4];
    DecPicAlignment align;

    /* compressor bypass flag */
    u8 compress_bypass;

    /* ringbuffer mode by default */
    u8 is_ringbuffer;
    u32 tile_by_tile;

    /* PP in standalone mode */
    u32 pp_standalone;

    /*only used for cache&shaper rtl simulation*/
    u32 cache_bypass;
    u32 shaper_bypass;
    u32 cache_enable;
    u32 shaper_enable;
    u32 stripe_enable;

    /* output format conversion enabled in TB (via -T/-S/-P options) */
    u32 out_format_conv;
    u32 mc_enable;
    u32 width;
    u32 height;
    int in_10bit;
    char *in_format;
    int priority;
    char *device;
    int mem_id;
    int ext_buffers_need;
    int ext_buffers_need_minus_dpb;
} VpiPPParams;

typedef struct {
    struct VpiPPParams param;
    void *dwl;
    PPInst *pp_inst;
    VpiPPConfig pp_config;
    PPConfig dec_cfg;
    u32 num_of_output_pics;
    int pp_enabled;

    struct RawParser *rawparse_ctx;

#ifdef SUPPORT_TCACHE
    TcacheContext tcache_config;
    struct DWLLinearMem edma_link;
#endif

    struct DecInput *pp_in_buffer;

    int max_frames_delay;
    u32 out_buf_size;
    u32 out_buf_nums;
    u32 out_buf_nums_init;
    struct DWLLinearMem pp_out_buffer[OUTPUT_BUF_DEPTH];
    FifoInst pp_out_Fifo;
#ifdef SUPPORT_DEC400
    u32 dec_table_offset;
#endif
    const void *trans_handle;
    EDMA_HANDLE edma_handle;
    const void *tcache_handle;
} PPClient;

typedef struct ppResize {
    int x;
    int y;
    int cw;
    int ch;
    int sw;
    int sh;
} PPResize;

typedef struct VpiPPFilter {
    int nb_outputs;
    char *low_res;
    int buffer_depth;
    int vce_ds_enable;

    PPResize resizes[4];
    int low_res_num;

    int force_10bit;
    VpiPPParams params;

    const void *inst;
    const void *mwl;
    struct DecInput buffers;

    PPClient *pp_client;

#ifdef PP_MEM_ERR_TEST
    int memory_err_shadow;
#endif

#ifdef PP_EDMA_ERR_TEST
    int edma_err_shadow;
#endif
    int w;
    int h;
    VpiPixsFmt format;
    VpiFrame *frame;
} VpiPPFilter;

#endif
