#include <node_api.h>
#include <napi-macros.h>
#include <uv.h>

#ifndef WIN32
#include <sys/file.h>
#include <fcntl.h>
#endif

NAPI_METHOD(fsctl_native_sparse) {
  NAPI_ARGV(1)
  NAPI_ARGV_UINT32(fd, 0)

  #ifdef WIN32
  uv_os_fd_t h = uv_get_osfhandle(fd);
  DWORD d;
  int result = DeviceIoControl(h, FSCTL_SET_SPARSE, NULL, 0, NULL, 0, &d, NULL) ? 1 : 0;
  #else
  int result = 1;
  #endif

  NAPI_RETURN_UINT32(result)
}

NAPI_METHOD(fsctl_native_holepunch) {
  NAPI_ARGV(3)
  NAPI_ARGV_UINT32(fd, 0)
  NAPI_ARGV_UINT32(start, 1)
  NAPI_ARGV_UINT32(end, 2)

  uv_os_fd_t h = uv_get_osfhandle(fd);

  #ifdef WIN32
  DWORD d;
  LARGE_INTEGER _start;
  _start.QuadPart = start;
  LARGE_INTEGER _end;
  _end.QuadPart = end;
  FILE_ZERO_DATA_INFORMATION _info;
  _info.FileOffset = _start;
  _info.BeyondFinalZero = _end;
  int result = DeviceIoControl(h, FSCTL_SET_ZERO_DATA, &_info, sizeof(_info), NULL, 0, &d, NULL) ? 1 : 0;
  #else
  int result = fallocate(h, FALLOC_FL_PUNCH_HOLE, start, end) == 0 ? 1 : 0;
  #endif

  NAPI_RETURN_UINT32(result)
}

NAPI_METHOD(fsctl_native_lock) {
  NAPI_ARGV(1)
  NAPI_ARGV_UINT32(fd, 0)

  uv_os_fd_t h = uv_get_osfhandle(fd);

  #ifdef WIN32
  int locked = LockFile(h, 0, 0, 1, 0) ? 1 : 0;
  #else
  int locked = flock(h, LOCK_EX | LOCK_NB) == 0 ? 1 : 0;
  #endif

  NAPI_RETURN_UINT32(locked)
}

NAPI_METHOD(fsctl_native_unlock) {
  NAPI_ARGV(1)
  NAPI_ARGV_UINT32(fd, 0)

  uv_os_fd_t h = uv_get_osfhandle(fd);

  #ifdef WIN32
  int unlocked = UnlockFile(h, 0, 0, 1, 0) ? 1 : 0;
  #else
  int unlocked = flock(h, LOCK_UN | LOCK_NB) == 0 ? 1 : 0;
  #endif

  NAPI_RETURN_UINT32(unlocked)

}

NAPI_INIT () {
  NAPI_EXPORT_FUNCTION(fsctl_native_sparse)
  NAPI_EXPORT_FUNCTION(fsctl_native_lock)
  NAPI_EXPORT_FUNCTION(fsctl_native_unlock)
  NAPI_EXPORT_FUNCTION(fsctl_native_holepunch)
}
