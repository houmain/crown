
#include "audio.h"
#include "platform/platform.h"

#define STB_VORBIS_NO_STDIO
#define STB_VORBIS_NO_PULLDATA_API
#define STB_VORBIS_NO_INTEGER_CONVERSION
#define STB_VORBIS_MAX_CHANNELS 2
#include "stb/stb_vorbis.c"

#include <map>
#include <cstring>

namespace {
  AudioBuffer load_ogg_vorbis_mono(const Asset& asset) {
    const auto data = asset.data;
    const auto size = static_cast<int>(asset.size);

    auto samples = std::add_pointer_t<float>{ };

    // decode twice, first pass to calculate sample count and allocate buffer
    for (auto pass = 0; ; ++pass) {
      auto offset = 0;
      auto error = 0;
      auto vorbis = stb_vorbis_open_pushdata(data, size, &offset, &error, nullptr);
      assert(vorbis);

      auto sample_count = 0;
      auto channels = 0;
      auto channel_samples = std::add_pointer_t<float*>{ };
      auto total_sample_count = 0;

      while (offset < size) {
        const auto used = stb_vorbis_decode_frame_pushdata(vorbis,
          data + offset, size - offset, &channels,
          &channel_samples, &sample_count);
        assert(used);

        if (pass == 1 && sample_count)
          std::memcpy(samples + total_sample_count,
            channel_samples[0], sizeof(float) * sample_count);

        total_sample_count += sample_count;
        offset += used;
      }

      stb_vorbis_close(vorbis);

      if (pass == 1)
        return {
          std::shared_ptr<const float>(samples,
            [](const float* samples) { delete[] samples; }),
          total_sample_count
        };

      samples = new float[total_sample_count];
    }
  }

  AudioBuffer get_audio_buffer(const Asset& asset) {
    static auto s_cache = std::map<const Asset*, AudioBuffer, std::less<>>();
    auto it = s_cache.find(&asset);
    if (it == s_cache.end())
      it = s_cache.emplace(&asset, load_ogg_vorbis_mono(asset)).first;
    return it->second;
  }
} // namespace

void play_audio(const Asset& asset, float volume) {
  platform_play_audio(get_audio_buffer(asset), volume, volume);
}
