/*------------------------------------------------------------------------------
--                                                                                                                               --
--       This software is confidential and proprietary and may be used                                   --
--        only as expressly authorized by a licensing agreement from                                     --
--                                                                                                                               --
--                            Verisilicon.                                                                                    --
--                                                                                                                               --
--                   (C) COPYRIGHT 2014 VERISILICON                                                            --
--                            ALL RIGHTS RESERVED                                                                    --
--                                                                                                                               --
--                 The entire notice above must be reproduced                                                 --
--                  on all copies and should not be removed.                                                    --
--                                                                                                                               --
--------------------------------------------------------------------------------
--
-- Description : Hevc SEI Messages.
--
------------------------------------------------------------------------------*/

#ifndef HEVC_SEI_H
#define HEVC_SEI_H

#include "base_type.h"
#include "sw_put_bits.h"
#include "enccommon.h"
#include "sw_parameter_set.h"


typedef struct
{
  u32 fts;    /* Full time stamp */
  u32 timeScale;
  u32 nuit;   /* number of units in tick */
  u32 time;   /* Modulo time */
  u32 secf;
  u32 sec;    /* Seconds */
  u32 minf;
  u32 min;    /* Minutes */
  u32 hrf;
  u32 hr; /* Hours */
} timeStamp_s;

typedef struct
{
  timeStamp_s ts;
  u32 nalUnitSize;
  u32 enabled;
  true_e byteStream;
  u32 hrd;    /* HRD conformance */
  u32 seqId;
  u32 icrd;   /* initial cpb removal delay */
  u32 icrdLen;
  u32 icrdo;  /* initial cpb removal delay offset */
  u32 icrdoLen;
  u32 crd;    /* CPB removal delay */
  u32 crdLen;
  u32 dod;    /* DPB removal delay */
  u32 dodLen;
  u32 psp;
  u32 ps;
  u32 cts;
  u32 cntType;
  u32 cdf;
  u32 nframes;
  u32 toffs;
  u32 toffsLen;
  u32 userDataEnabled;
  const u8 *pUserData;
  u32 userDataSize;
  u32 activated_sps;
  u32 insertRecoveryPointMessage;
  u32 recoveryFrameCnt;

  u32 hdr10_display_enable;
  u32 hdr10_dx0;
  u32 hdr10_dy0;
  u32 hdr10_dx1;
  u32 hdr10_dy1;
  u32 hdr10_dx2;
  u32 hdr10_dy2;
  u32 hdr10_wx;
  u32 hdr10_wy;
  u32 hdr10_maxluma;
  u32 hdr10_minluma;
  u8  hdr10_lightlevel_enable;
  u16 hdr10_maxlight;
  u16 hdr10_avglight;
} sei_s;

void HevcInitSei(sei_s *sei, true_e byteStream, u32 hrd, u32 timeScale,
                 u32 nuit);
void HevcUpdateSeiTS(sei_s *sei, u32 timeInc);

void HevcFillerSei(struct buffer *sp, sei_s *sei, i32 cnt);
void HevcBufferingSei(struct buffer *sp, sei_s *sei, vui_t *vui);
void HevcPicTimingSei(struct buffer *sp, sei_s *sei, vui_t *vui);
void HevcUserDataUnregSei(struct buffer *sp, sei_s *sei);
void HevcUpdateSeiPS(sei_s *sei, u32 interlacedFrame, u32 bottomfield);
void HevcActiveParameterSetsSei(struct buffer *sp, sei_s *sei, vui_t *vui);
void HevcRecoveryPointSei(struct buffer *sp, sei_s *sei);
void H264ScalabilityInfoSei(struct buffer * sp, struct sps *s, i32 svctLevel, i32 frameRate);

void HevcUpdateSeiMasteringDisplayColour(sei_s *sei, Hdr10DisplaySei *pDisplaySei);
void HevcMasteringDisplayColourSei(struct buffer *sp, sei_s *sei);
void HevcUpdateSeiContentLightLevelInfo(sei_s *sei, Hdr10LightLevelSei *pLightSei);
void HevcContentLightLevelSei(struct buffer *sp, sei_s *sei);

void H264InitSei(sei_s * sei, true_e byteStream, u32 hrd, u32 timeScale, u32 nuit);
void H264UpdateSeiTS(sei_s * sei, u32 timeInc);
void H264FillerSei(struct buffer *sp, sei_s * sei, i32 cnt);
void H264BufferingSei(struct buffer * sp, sei_s * sei);
void H264PicTimingSei(       struct buffer * sp, sei_s * sei);
void H264UserDataUnregSei(struct           buffer * sp, sei_s * sei);
void H264RecoveryPointSei(struct buffer * sp, sei_s * sei);



#endif
