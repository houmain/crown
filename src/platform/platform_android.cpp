
#include "platform.h"

#include <android_native_app_glue.h>
#include <jni.h>
#include <android/native_activity.h>
#include <EGL/egl.h>

void handle_cmd(android_app* app, int32_t cmd) {
}

int32_t handle_input(android_app* app, AInputEvent* event) {
  return 0;
}

void android_main(android_app* app) {
  app->onAppCmd = handle_cmd;
	app->onInputEvent = handle_input;
}
