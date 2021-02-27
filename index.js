const binding = require('node-gyp-build')(__dirname)

module.exports = {
  lock (fd) {
    return binding.fsctl_native_lock(fd) > 0
  },
  unlock (fd) {
    return binding.fsctl_native_unlock(fd) > 0
  },
  sparse (fd) {
    return binding.fsctl_native_sparse(fd) > 0
  },
  punch (fd, start, end) {
    return binding.fsctl_native_holepunch(fd, start, end) > 0
  }
}
