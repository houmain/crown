
#include "platform.h"

# include <wajic_gl.h>

WAJIC(void, WASetupCanvas, (int width, int height), {
  const canvas = WA.canvas;
  canvas.width = width;
  canvas.height = height;

  const startTime = performance.now();
  const drawFunc = function() {
    if (STOP)
      return;
    window.requestAnimationFrame(drawFunc);
    ASM.WAFNDraw(canvas.width, canvas.height, performance.now() - startTime);
  };
  window.requestAnimationFrame(drawFunc);
})

WAJIC(void, WASetupInput, (), {
  const canvas = WA.canvas;
  const cancelEvent = function(e) { if (e.preventDefault) e.preventDefault(true); else if (e.stopPropagation) e.stopPropagation(true); else e.stopped = true; };
  const windowEvent = function(t, f) { window.addEventListener(t, f, true); };
  const canvasEvent = function(t, f) { canvas.addEventListener(t, f, { capture:true, passive:false }); };
  const getKeyCode = function(key) {
    if (key.length == 1)
      return key.toUpperCase().charCodeAt();
    switch (key) {
      case 'ArrowRight': return 262;
      case 'ArrowLeft': return 263;
      case 'Arrowdown': return 264;
      case 'ArrowUp': return 265;
    }
    return 0;
  };
  windowEvent('keydown', function(e) {
    ASM.WAFNKey(true, getKeyCode(e.key));
    if (e.key.length == 1) ASM.WAFNText(e.key.charCodeAt());
    cancelEvent(e);
  });
  windowEvent('keyup', function(e) {
    ASM.WAFNKey(false, getKeyCode(e.key));
    cancelEvent(e);
  });
  canvasEvent('mousemove', function(e) {
    ASM.WAFNMouseMove(e.offsetX * canvas.width / canvas.clientWidth , e.offsetY * canvas.height / canvas.clientHeight);
    cancelEvent(e);
  });
  let buttons = 0;
  canvasEvent('mousedown', function(e) {
    var btn = (1<<e.button);
    if (buttons & btn) return;
    buttons |= btn;
    ASM.WAFNMouseButton(e.button, true);
    cancelEvent(e);
  });
  windowEvent('mouseup', function(e) {
    var btn = (1<<e.button);
    if (!(buttons & btn)) return;
    buttons &= ~btn;
    ASM.WAFNMouseButton(e.button, false);
    cancelEvent(e);
  });
  canvasEvent('wheel', function(e) { ASM.WAFNMouseWheel(e.deltaX, e.deltaY); cancelEvent(e); });
  canvasEvent('DOMMouseScroll', function(e) { ASM.WAFNMouseWheel(-e.detail * 40); cancelEvent(e); });
})

WAJIC(void, WASetupAudio, (), {
  try { WA.audio = new AudioContext(); } catch (e) { }
  if (!WA.audio) { WA.print('Warning: WebAudio not supported\n'); return; }

  const frameCount = 0.2 * WA.audio.sampleRate;
  let samples = ASM.malloc(frameCount * 4 * 2);
  let f32samples = samples / 4;
  let encTime = 0;

  const streamFunc = function() {
    if (STOP) {
      WA.audio.close();
      return;
    }

    ASM.WAFNMusic(samples, frameCount);
    let buffer = WA.audio.createBuffer(2, frameCount, WA.audio.sampleRate);
    buffer.getChannelData(0).set(MF32.subarray(f32samples, f32samples + frameCount));
    buffer.getChannelData(1).set(MF32.subarray(f32samples + frameCount, f32samples + 2 * frameCount));

    let source = WA.audio.createBufferSource();
    source.buffer = buffer;
    source.connect(WA.audio.destination);
    encTime = Math.max(encTime, WA.audio.currentTime);
    source.start(encTime);
    encTime += frameCount / WA.audio.sampleRate;

    window.setTimeout(streamFunc, (encTime - WA.audio.currentTime - 0.1) * 1000);
  };
  streamFunc();
})

WAJIC(void, WAPlayAudio, (const float* samples, int sampleCount), {
  let f32samples = samples / 4;

  let buffer = WA.audio.createBuffer(2, sampleCount, WA.audio.sampleRate);
  buffer.getChannelData(0).set(MF32.subarray(f32samples, f32samples + sampleCount));
  buffer.getChannelData(1).set(MF32.subarray(f32samples, f32samples + sampleCount));

  let source = WA.audio.createBufferSource();
  source.buffer = buffer;
  source.connect(WA.audio.destination);
  source.start();
})

WA_EXPORT(WAFNDraw) void WAFNDraw(int width, int height, int t) {
  platform_on_update(width, height, t / 1000.0);
}

WA_EXPORT(WAFNKey) void WAFNKey(int is_down, int key_code) {
  platform_on_key(static_cast<KeyCode>(key_code), static_cast<bool>(is_down));
}

WA_EXPORT(WAFNText) void WAFNText(unsigned int code) {
  platform_on_text(code >= ' ' && code <= '~' ? static_cast<char>(code) : '?');
}

WA_EXPORT(WAFNMouseButton) void WAFNMouseButton(int button, int is_down) {
  platform_on_mouse_button(button, static_cast<bool>(is_down));
}

WA_EXPORT(WAFNMouseMove) void WAFNMouseMove(int x, int y) {
  platform_on_mouse_move(x, y);
}

WA_EXPORT(WAFNMouseWheel) void WAFNMouseWheel(float deltax, float deltay) {
  platform_on_mouse_wheel(deltax, deltay);
}

WA_EXPORT(WAFNMusic) void WAFNMusic(float* samples, int frameCount) {
  platform_music_callback(samples, frameCount);
}

void platform_error(const char* message) {
  std::fprintf(stderr, "%s", message);
  std::abort();
}

void platform_play_audio(AudioBuffer buffer) {
  WAPlayAudio(buffer.samples.get(), buffer.sample_count);
}

int main() {
  WASetupCanvas(platform_minimum_width, platform_minimum_height);
  WASetupInput();
  WASetupAudio();
  glSetupCanvasContext(0, 0, 0, 0);
  platform_on_setup();
}
