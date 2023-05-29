#include "server.cpp"
class ThreadPool
{
public:
  ThreadPool(int count)
  {
    this->lock = new pthread_mutex_t();
    this->cond = new pthread_cond_t();
    if (pthread_mutex_init(lock, nullptr) < 0)
    {
      std::cerr << "Couldn't init mutex";
    }

    if (pthread_cond_init(cond, nullptr) < 0)
    {
      std::cerr << "Couldn't init cond_var";
    }
    this->count = count;
    this->threads = new pthread_t[this->count];
    for (int i = 0; i < this->count; i++)
    {
      pthread_create(&threads[i], nullptr, execute_task, this);
    }
  }

  ~ThreadPool()
  {
    pthread_cond_destroy(cond);
    pthread_mutex_destroy(lock);
    for (int i = 0; i < this->count; i++)
    {
      pthread_kill(threads[i], SIGKILL);
    }
    delete[] threads;
  }

  void addTask(Task *task)
  {
    pthread_mutex_lock(lock);
    this->taskQueue.push(task);
    pthread_mutex_unlock(lock);
    pthread_cond_signal(cond);
  }

  static void *execute_task(void *arg)
  {

    ThreadPool *threadpool = (ThreadPool*)arg;

    while (true)
    {
      pthread_mutex_lock(threadpool->lock);
      while (threadpool->taskQueue.empty())
      {
        pthread_cond_wait(threadpool->cond, threadpool->lock);
      }
      Task *task = threadpool->taskQueue.front();
      threadpool->taskQueue.pop();
      pthread_mutex_unlock(threadpool->lock);
    }
  }
private:
  std::queue<Task*> taskQueue;
  pthread_t* threads;
  int count;
  pthread_mutex_t *lock;
  pthread_cond_t *cond;
};