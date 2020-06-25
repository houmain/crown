
#include "framework/get_resource.h"

extern "C" {
#include "libxm/xm.h"
}

void platform_music_callback(float* output_frames, int frame_count) {
#if defined(NDEBUG)
  static xm_context_t* xm_context;
  if (!xm_context) {
    auto xm = get_resource("dualtrax.xm");
    xm_create_context_safe(&xm_context,
      reinterpret_cast<const char*>(xm.first), xm.second, 44100);
  }
  xm_generate_samples(xm_context, output_frames, frame_count);
#endif
}
