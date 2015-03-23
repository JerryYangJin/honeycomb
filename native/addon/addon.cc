//#include "wrapper.h"
#include <v8.h>
#include <node.h>

#include "nan.h"

using namespace node;
using namespace v8;

#include "git_backend.h"
#include "git_mysql.h"

void init(Handle<v8::Object> target) {
  NanScope();

  GitBackend::InitializeComponent(target);
  GitMysql::InitializeComponent(target);
}

NODE_MODULE(addon, init)
