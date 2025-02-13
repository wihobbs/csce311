// Copyright 2022 CSCE 311
//

#include <shm_logger/inc/consumer.h>


namespace logger {

Consumer::Consumer(const char* shm_name, const char* shm_log_signal_name)
    : shm_name_(shm_name),
      shm_log_signal_(shm_log_signal_name) {
  // open shared memory and capture file descriptor
  int shm_fd = ::shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, 0660);
  if (shm_fd < 0) {
    std::cerr << ::strerror(errno) << std::endl;

    ::exit(errno);
  }

  // set size of shared memory with file descriptor
  const size_t kBuffer_size = 4096 - sizeof(size_t);  // page - buffer_size size
  const size_t kSHM_size = sizeof(SharedMemoryStore) + kBuffer_size;
  if (::ftruncate(shm_fd, kSHM_size) < 0) {
    std::cerr << ::strerror(errno) << std::endl;

    ::exit(errno);
  }

  // get copy of mapped mem
  const int kProt = PROT_READ | PROT_WRITE;
  store_ = static_cast<SharedMemoryStore*>(
    ::mmap(nullptr, sizeof(SharedMemoryStore), kProt, MAP_SHARED, shm_fd, 0));

  if (store_ == MAP_FAILED) {
    std::cerr << ::strerror(errno) << std::endl;

    ::exit(errno);
  }

  // init memory map
  *store_ = {};
  store_->buffer_size = kBuffer_size;  // set store's buffer size

  // create signal mux (unlocked by producer(s))
  shm_log_signal_.Create(0);
  shm_log_signal_.Open();
}


Consumer::~Consumer() {
  // return copy of mapped mem, capture any error/exit code
  int exit_code = ::munmap(store_, sizeof(SharedMemoryStore));

  // alert for error in ::munmap
  if (exit_code < 0)
    std::cerr << ::strerror(errno) << std::endl;

  // delete shared memory map
  if (::shm_unlink(shm_name_) < 0)
    std::cerr << ::strerror(errno) << std::endl;

  // delete named semaphore
  shm_log_signal_.Destroy();

  ::exit(errno);
}


void Consumer::Consume(const char log_name[]) {
  // write any logs to file
  std::string msg;
  while (true) {
    shm_log_signal_.Down();  // block until occupied signal
    std::ofstream fout(log_name, std::ofstream::app);

    fout << store_->buffer << '\n';  // write to file

    fout.close();
  }
}


}  // namespace logger
