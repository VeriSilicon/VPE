/*
 * Copyright 2020 VeriSilicon, Inc.
 *
 *	The program is distributed under terms of BSD license.
 *	You can obtain the copy of the license by visiting:
 *
 *	http://www.opensource.org/licenses/bsd-license.php
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __VPI_TYPES_H__
#define __VPI_TYPES_H__

#include <stdint.h>

#define PIC_INDEX_MAX_NUMBER 5

#define VPE_TASK_LIVE        0
#define VPE_TASK_VOD         1

typedef void *VpiCtx;

typedef enum {
    VPI_CMD_BASE,

    /*decoder command*/
    VPI_CMD_DEC_PIC_CONSUME,
    VPI_CMD_DEC_STRM_BUF_COUNT,
    VPI_CMD_DEC_GET_USED_STRM_MEM,
    VPI_CMD_DEC_SET_FRAME_BUFFER,
    VPI_CMD_DEC_GET_STRM_BUF_PKT,
    VPI_CMD_DEC_INIT_OPTION,

    /*hw downloader command*/
    VPI_CMD_HWDW_FREE_BUF,
    VPI_CMD_HWDW_SET_INDEX,

    /*vp9 encoder command*/
    VPI_CMD_VP9ENC_SET_PENDDING_FRAMES_COUNT,
    VPI_CMD_VP9ENC_GET_NEXT_PIC,
    VPI_CMD_VP9ENC_GET_PIC_TOBE_FREE,

    /*H26x encoder command*/
    VPI_CMD_H26xENC_GET_NEXT_PIC,
    VPI_CMD_H26xENC_SET_FINDPIC,
    VPI_CMD_H26xENC_GET_FLUSHSTATE,
    VPI_CMD_H26xENC_UPDATE_STATISTIC,
    VPI_CMD_H26xENC_SET_NO_INFRM,

    /*pp command*/
    VPI_CMD_PP_CONFIG,
    VPI_CMD_PP_CONSUME,

    /*vpe hwcontext pool buffer command*/
    VPI_CMD_GET_FRAME_BUFFER,
    VPI_CMD_FREE_FRAME_BUFFER,

    /*common command*/
    VPI_CMD_GET_VPEFRAME_SIZE,
    VPI_CMD_GET_PICINFO_SIZE,
} VpiCmd;

typedef enum VpiPixsFmt {
    VPI_FMT_YUV420P,
    VPI_FMT_NV12,
    VPI_FMT_NV21,
    VPI_FMT_YUV420P10LE,
    VPI_FMT_YUV420P10BE,
    VPI_FMT_P010LE,
    VPI_FMT_P010BE,
    VPI_FMT_YUV422P,
    VPI_FMT_YUV422P10LE,
    VPI_FMT_YUV422P10BE,
    VPI_FMT_YUV444P,
    VPI_FMT_RGB24,
    VPI_FMT_BGR24,
    VPI_FMT_ARGB,
    VPI_FMT_RGBA,
    VPI_FMT_ABGR,
    VPI_FMT_BGRA,
} VpiPixsFmt;

typedef struct VpiEncParamSet {
    char *key;
    char *value;
    struct VpiEncParamSet *next;
}VpiEncParamSet;

/**
 * VP9 encoder opition data which got from application like ffmpeg
 */
typedef struct VpiEncVp9Opition {
    int task_id;
    int priority;
    char *dev_name;
    int force_8bit;
    int width;
    int height;

    /*low level hardware frame context, point to one VpiFrame pointer*/
    void *framectx;

    /*Parameters which copied from ffmpeg AVCodecContext*/
    uint32_t bit_rate;
    int bit_rate_tolerance;
    int frame_rate_numer;
    int frame_rate_denom;

    /*VPE VP9 encoder public parameters*/
    char *preset;
    int effort;
    int lag_in_frames;
    int passes;

    /*VPE VP9 encoder public parameters with -enc_params*/
    VpiEncParamSet *param_list;
} VpiEncVp9Opition;

/**
 * PP configuration
 */
typedef struct VpiPPOpition {
    int nb_outputs;
    int force_10bit;
    char *low_res;
    int w;
    int h;
    VpiPixsFmt format;
    /*low level hardware frame context, point to one VpiFrame pointer*/
    void *frame;
} VpiPPOpition;

typedef struct VpiPacket {
    uint32_t size;
    uint8_t *data;
    int64_t pts;
    int64_t pkt_dts;
    int32_t index_encoded;
    void *opaque;
} VpiPacket;

typedef struct VpiCrop{
    int enabled;
    int x;
    int y;
    int w;
    int h;
} VpiCrop;

typedef struct VpiScale{
    int enabled;
    int w;
    int h;
} VpiScale;

typedef struct VpiPicData{
    unsigned int is_interlaced;
    unsigned int pic_stride;
    unsigned int crop_out_width;
    unsigned int crop_out_height;
    unsigned int pic_format;
    unsigned int pic_pixformat;
    unsigned int bit_depth_luma;
    unsigned int bit_depth_chroma;
    unsigned int pic_compressed_status;
} VpiPicData;

typedef struct VpiPicInfo{
    int enabled;
    int flag;
    int format;
    int width;
    int height;
    int pic_width;
    int pic_height;
    VpiCrop crop;
    VpiScale scale;
    VpiPicData picdata; /* add for get init param*/
} VpiPicInfo;

typedef struct VpiFrame {
    int task_id;
    int src_width;
    int src_height;
    int width;
    int height;

    /* For video the linesizes should be multiples of the CPUs alignment
     * preference, this is 16 or 32 for modern desktop CPUs.
     * Some code requires such alignment other code can be slower without
     * correct alignment, for yet other it makes no difference.*/
    int linesize[3];
    int key_frame;
    int64_t pts;
    int64_t pkt_dts;
    uint8_t *data[3];
    uint32_t pic_struct_size;
    VpiPicInfo pic_info[PIC_INDEX_MAX_NUMBER];
    /* Manage the frame buffer pool buffers.
     * Locked status shows the frame buffer using status
     * It will be locked by decoder, and unlocked by encoder. */
    int locked;
    /* the output channel count that spliter outputs */
    int nb_outputs;
    /* Number of same Buffer used by multiple channel encoders */
    int used_cnt;
    /* number of ext frame buffer for transcoding case */
    int max_frames_delay;
    void *opaque;
    void (*vpe_frame_free)(void *opaque, uint8_t *data);
} VpiFrame;

typedef struct VpiSysInfo {
    int device;
    char *device_name;
    int sys_log_level;
    int task_id;
    int priority;
} VpiSysInfo;

typedef struct VpiCtrlCmdParam {
    VpiCmd cmd;
    void *data;
} VpiCtrlCmdParam;

typedef enum VpiPlugin {
    H264DEC_VPE,
    HEVCDEC_VPE,
    VP9DEC_VPE,
    H26XENC_VPE,
    VP9ENC_VPE,
    PP_VPE,
    SPLITER_VPE,
    HWDOWNLOAD_VPE,
    HWCONTEXT_VPE,
} VpiPlugin;

/*
 * below definition is for H26xEnc
 */
typedef enum VpiH26xCodecID {
    CODEC_ID_HEVC,
    CODEC_ID_H264,
} VpiH26xCodecID;

typedef enum VpiH26xPreset {
    VPE_PRESET_NONE,
    VPE_PRESET_SUPERFAST,
    VPE_PRESET_FAST,
    VPE_PRESET_MEDIUM,
    VPE_PRESET_SLOW,
    VPE_PRESET_SUPERSLOW,
    VPE_PRESET_NUM
} VpiH26xPreset;

typedef enum VpiFlushState {
    VPIH26X_FLUSH_IDLE,
    VPIH26X_FLUSH_PREPARE,
    VPIH26X_FLUSH_TRANSPIC,
    VPIH26X_FLUSH_ENCDATA,
    VPIH26X_FLUSH_FINISH,
} VpiFlushState;

typedef enum VpiPixFmt{
    VPI_YUV420_PLANAR,
    VPI_YUV420_SEMIPLANAR,
    VPI_YUV420_SEMIPLANAR_VU,
    VPI_YUV420_PLANAR_10BIT_P010,
} VpiPixFmt;

typedef enum VpiEncRet {
    VPI_ENC_START_OK = 100, /*OK returned at encoder's start stage*/
    VPI_ENC_START_OUT_ES, /*OUT_ES returned at encoder's start stage*/
    VPI_ENC_START_ERROR, /*ERROR returned at encoder's stage stage*/
    VPI_ENC_ENC_OK, /*OK returned at encoder's encoding stage*/
    VPI_ENC_ENC_READY, /*READY returned at encoder's encoding stage*/
    VPI_ENC_ENC_FRM_ENQUEUE, /*FRM_ENQUEUE returned at encoder's encoding
                                stage*/
    VPI_ENC_ENC_ERROR, /*ERROR returned at encoder's encoding stage*/
    VPI_ENC_FLUSH_IDLE_OK, /*OK returned at encoder's FLUSH_IDLE stage */
    VPI_ENC_FLUSH_IDLE_READY, /*READY returned at encoder's FLUSH_IDLE stage*/
    VPI_ENC_FLUSH_IDLE_FRM_ENQUEUE, /*RFM_ENQUEUE returned at encoder's
                                       FLUSH_IDLE stage*/
    VPI_ENC_FLUSH_IDLE_ERROR, /*ERROR returned at encoder's FLUSH_IDLE stage*/
    VPI_ENC_FLUSH_PREPARE, /*indicate the encoder at FLUSH_PREPARE stage*/
    VPI_ENC_FLUSH_TRANSPIC_OK, /*OK returned at encoder's FLUSH_TRANSPIC stage*/
    VPI_ENC_FLUSH_TRANSPIC_READY, /*READY returned at encoder's FLUSH_TRANSPIC
                                     stage*/
    VPI_ENC_FLUSH_TRANSPIC_ERROR, /*READY returned at encoder's FLUSH_TRANSPIC
                                     stage*/
    VPI_ENC_FLUSH_ENCDATA_OK, /*OK returned at encoder's FLUSH_ENCDATA stage*/
    VPI_ENC_FLUSH_ENCDATA_READY, /*READY returned at encoder's FLUSH_ENCDATA
                                    stage*/
    VPI_ENC_FLUSH_ENCDATA_FRM_ENQUEUE, /*FRM_ENQUEUE returned at encoder's
                                          FLUSH_ENCDATA stage*/
    VPI_ENC_FLUSH_ENCDATA_ERROR, /*ERROR returned at encoder's FLUSH_ENCDATA
                                    stage*/
    VPI_ENC_FLUSH_FINISH_OK, /*OK returned at encoder's FLUSH_FINISH stage*/
    VPI_ENC_FLUSH_FINISH_ERROR, /*ERROR returned at encoder's FLUSH_FINISH
                                   stage*/
    VPI_ENC_FLUSH_FINISH_END /*END returned at encoder's FLUSH_FINISH stage*/
} VpiEncRet;

typedef struct DecOption {
    char *pp_setting;
    char *dev_name;
    int buffer_depth;
    int transcode;
    int task_id;
    int priority;
    VpiFrame *frame;
} DecOption;

typedef struct H26xEncCfg {
    char module_name[20];
    int pp_index;
    int priority;
    char *device;
    // int task_id;
    int crf;
    char *preset;
    VpiH26xCodecID codec_id;
    const char *codec_name;
    char *profile;
    char *level;
    int bit_per_second;
    int input_rate_numer; /* Input frame rate numerator */
    int input_rate_denom; /* Input frame rate denominator */
    int lum_width_src;
    int lum_height_src;

    /*VCENC_YUV420_PLANAR,VCENC_YUV420_SEMIPLANAR,VCENC_YUV420_SEMIPLANAR_VU,
       VCENC_YUV420_PLANAR_10BIT_P010,VCENC_YUV420_PLANAR*/
    int input_format;
    VpiFrame *frame_ctx;

    /*VPE H26x encoder public parameters with -enc_params*/
    VpiEncParamSet *param_list;
} H26xEncCfg;

typedef struct VpiApi {
    int (*init)(VpiCtx, void *);

    int (*decode)(VpiCtx, void *indata, void *outdata);

    int (*encode)(VpiCtx, void *indata, void *outdata);

    int (*decode_put_packet)(VpiCtx, void *indata);

    int (*decode_get_frame)(VpiCtx, void *outdata);

    int (*encode_put_frame)(VpiCtx, void *indata);

    int (*encode_get_packet)(VpiCtx, void *outdata);

    int (*control)(VpiCtx, void *indata, void *outdata);

    int (*process)(VpiCtx, void *indata, void *outdata);

    int (*close)(VpiCtx);
} VpiApi;

typedef struct VpiMediaProc {
    VpiCtx ctx;
    VpiApi *vpi;
} VpiMediaProc;

#endif
