
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
  windowEvent('keydown', function(e) {
    ASM.WAFNKey(true, e.keyCode);
    if (e.key.length == 1) ASM.WAFNText(e.key.charCodeAt());
    cancelEvent(e);
  });
  windowEvent('keyup', function(e) {
    ASM.WAFNKey(false, e.keyCode);
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
  let audioCtx;
  try { audioCtx = new AudioContext(); } catch (e) { }
  if (!audioCtx) { WA.print('Warning: WebAudio not supported\n'); return; }

  const frameCount = 0.2 * audioCtx.sampleRate;
  let samples = ASM.malloc(frameCount * 4 * 2);
  let f32samples = samples / 4;
  let encTime = 0;

  const streamFunc = function() {
    if (STOP) {
      audioCtx.close();
      return;
    }

    ASM.WAFNMusic(samples, frameCount);
    let buffer = audioCtx.createBuffer(2, frameCount, audioCtx.sampleRate);
    buffer.getChannelData(0).set(MF32.subarray(f32samples, f32samples + frameCount));
    buffer.getChannelData(1).set(MF32.subarray(f32samples + frameCount, f32samples + 2 * frameCount));

    let source = audioCtx.createBufferSource();
    source.buffer = buffer;
    source.connect(audioCtx.destination);
    encTime = Math.max(encTime, audioCtx.currentTime);
    source.start(encTime);
    encTime += frameCount / audioCtx.sampleRate;

    window.setTimeout(streamFunc, (encTime - audioCtx.currentTime - 0.1) * 1000);
  };
  streamFunc();
})

WA_EXPORT(WAFNDraw) void WAFNDraw(int width, int height, int t) {
  platform_on_update(width, height, t / 1000.0);
}

WA_EXPORT(WAFNKey) void WAFNKey(int is_down, int key_code) {
  platform_on_key(key_code, static_cast<bool>(is_down));
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

int main() {
  WASetupCanvas(platform_minimum_width, platform_minimum_height);
  WASetupInput();
  WASetupAudio();
  glSetupCanvasContext(0, 0, 0, 0);
  platform_on_setup();
}
