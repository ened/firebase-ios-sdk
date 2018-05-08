/*
 * Copyright 2018 Google
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FIRESTORE_CORE_SRC_FIREBASE_FIRESTORE_UTIL_EXECUTOR_LIBDISPATCH_H_
#define FIRESTORE_CORE_SRC_FIREBASE_FIRESTORE_UTIL_EXECUTOR_LIBDISPATCH_H_

#include <atomic>
#include <chrono>  // NOLINT(build/c++11)
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "dispatch/dispatch.h"

#include "Firestore/core/src/firebase/firestore/util/executor.h"
#include "Firestore/core/src/firebase/firestore/util/firebase_assert.h"
#include "absl/strings/string_view.h"

namespace firebase {
namespace firestore {
namespace util {

namespace internal {

// Generic wrapper over `dispatch_async_f`, providing `dispatch_async`-like
// interface: accepts an arbitrary invocable object in place of an Objective-C
// block.
void DispatchAsync(const dispatch_queue_t queue, std::function<void()>&& work);

// Similar to `DispatchAsync` but wraps `dispatch_sync_f`.
void DispatchSync(const dispatch_queue_t queue, std::function<void()> work);

class TimeSlot;

// A serial queue built on top of libdispatch. The operations are run on
// a dedicated serial dispatch queue.
class ExecutorLibdispatch : public Executor {
 public:
  ExecutorLibdispatch();
  explicit ExecutorLibdispatch(dispatch_queue_t dispatch_queue);
  ~ExecutorLibdispatch();

  bool IsCurrentExecutor() const override;
  std::string CurrentExecutorName() const override;
  std::string Name() const override;

  void Execute(Operation&& operation) override;
  void ExecuteBlocking(Operation&& operation) override;
  DelayedOperation Schedule(Milliseconds delay,
                            TaggedOperation&& operation) override;

  void RemoveFromSchedule(const TimeSlot* to_remove);

  bool IsScheduled(Tag tag) const override;
  absl::optional<TaggedOperation> PopFromSchedule() override;

  dispatch_queue_t dispatch_queue() const {
    return dispatch_queue_;
  }

 private:
  // GetLabel functions are guaranteed to never return a "null" string_view
  // (i.e. data() != nullptr).
  absl::string_view GetCurrentQueueLabel() const;
  absl::string_view GetTargetQueueLabel() const;

  std::atomic<dispatch_queue_t> dispatch_queue_;
  // Stores non-owned pointers to `TimeSlot`s.
  // Invariant: if a `TimeSlot` is in `schedule_`, it's a valid pointer.
  std::vector<TimeSlot*> schedule_;
};

}  // namespace internal
}  // namespace util
}  // namespace firestore
}  // namespace firebase

#endif  // FIRESTORE_CORE_SRC_FIREBASE_FIRESTORE_UTIL_EXECUTOR_LIBDISPATCH_H_
