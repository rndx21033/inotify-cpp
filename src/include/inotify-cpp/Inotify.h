/**
 * @file      Inotify.h
 * @author    Erik Zenker
 * @date      20.11.2017
 * @copyright MIT
 **/
#pragma once
#include <cassert>
#include <atomic>
#include <boost/bimap.hpp>
#include <chrono>
#include <queue>
#include <string>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <vector>

#include <inotify-cpp/FileSystemEvent.h>
#include <inotify-cpp/FileSystemAdapter.h>

#define MAX_EVENTS       4096
/**
 * MAX_EPOLL_EVENTS is set to 1 since there exists
 * only one eventbuffer. The value can be increased
 * when readEventsIntoBuffer can handle multiple
 * epoll events.
 */
#define MAX_EPOLL_EVENTS 1
#define EVENT_SIZE       (sizeof (inotify_event))

/**
 * @brief C++ wrapper for linux inotify interface
 * @class Inotify
 *        Inotify.h
 *        "include/Inotify.h"
 *
 * folders will be watched by watchFolderRecursively or
 * files by watchFile. If there are changes inside this
 * folder or files events will be raised. This events
 * can be get by getNextEvent.
 *
 * @eventMask
 *
 * IN_ACCESS         File was accessed (read) (*).
 * IN_ATTRIB         Metadata changed—for example, permissions,
 *                   timestamps, extended attributes, link count
 *                   (since Linux 2.6.25), UID, or GID. (*).
 * IN_CLOSE_WRITE    File opened for writing was closed (*).
 * IN_CLOSE_NOWRITE  File not opened for writing was closed (*).
 * IN_CREATE         File/directory created in watched directory(*).
 * IN_DELETE         File/directory deleted from watched directory(*).
 * IN_DELETE_SELF    Watched file/directory was itself deleted.
 * IN_MODIFY         File was modified (*).
 * IN_MOVE_SELF      Watched file/directory was itself moved.
 * IN_MOVED_FROM     Generated for the directory containing the old
 *                   filename when a file is renamed (*).
 * IN_MOVED_TO       Generated for the directory containing the new
 *                   filename when a file is renamed (*).
 * IN_OPEN           File was opened (*).
 * IN_ALL_EVENTS     macro is defined as a bit mask of all of the above
 *                   events
 * IN_MOVE           IN_MOVED_FROM|IN_MOVED_TO
 * IN_CLOSE          IN_CLOSE_WRITE | IN_CLOSE_NOWRITE
 *
 * See inotify manpage for more event details
 *
 */
namespace inotify {

class Inotify {
 public:
  Inotify();
  ~Inotify();
  void watchDirectoryRecursively(inotifypp::filesystem::path path);
  void watchFile(inotifypp::filesystem::path file);
  void unwatchFile(inotifypp::filesystem::path file);
  void ignoreFileOnce(inotifypp::filesystem::path file);
  void ignoreFile(inotifypp::filesystem::path file);
  void setEventMask(uint32_t eventMask);
  uint32_t getEventMask();
  void setEventTimeout(std::chrono::milliseconds eventTimeout, std::function<void(FileSystemEvent)> onEventTimeout);
  inotifypp::optional<FileSystemEvent> getNextEvent();
  void stop();
  bool hasStopped();

private:
  inotifypp::filesystem::path wdToPath(int wd);
  bool isIgnored(std::string file);
  bool isOnTimeout(const std::chrono::steady_clock::time_point &eventTime);
  void removeWatch(int wd);
  ssize_t readEventsIntoBuffer(std::vector<uint8_t>& eventBuffer);
  void readEventsFromBuffer(uint8_t* buffer, int length, std::vector<FileSystemEvent> &events);
  void filterEvents(std::vector<FileSystemEvent>& events, std::queue<FileSystemEvent>& eventQueue);
  void sendStopSignal();

private:
  int mError;
  std::chrono::milliseconds mEventTimeout;
  std::chrono::steady_clock::time_point mLastEventTime;
  uint32_t mEventMask;
  uint32_t mThreadSleep;
  std::vector<std::string> mIgnoredDirectories;
  std::vector<std::string> mOnceIgnoredDirectories;
  std::queue<FileSystemEvent> mEventQueue;
  boost::bimap<int, inotifypp::filesystem::path> mDirectorieMap;
  int mInotifyFd;
  std::atomic<bool> mStopped;
  int mEpollFd;
  epoll_event mInotifyEpollEvent;
  epoll_event mStopPipeEpollEvent;
  std::array<epoll_event, MAX_EPOLL_EVENTS> mEpollEvents;

  std::function<void(FileSystemEvent)> mOnEventTimeout;
  std::vector<uint8_t> mEventBuffer;

  std::array<int, 2> mStopPipeFd;
  const int mPipeReadIdx;
  const int mPipeWriteIdx;
};
}
