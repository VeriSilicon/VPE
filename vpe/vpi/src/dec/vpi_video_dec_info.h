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

#ifndef __VPI_VIDEO_DEC_API_H__
#define __VPI_VIDEO_DEC_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "vpi_video_dec.h"

void vpi_dec_performance_report(VpiDecCtx *vpi_ctx);
void vpi_report_dec_pic_info(VpiDecCtx *vpi_ctx, struct DecPicturePpu *picture);
const char *dec_ret_string(int retval);
void print_decode_return(int retval);

#ifdef __cplusplus
}
#endif

#endif