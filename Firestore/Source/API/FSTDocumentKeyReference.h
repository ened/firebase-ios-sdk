/*
 * Copyright 2019 Google
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

#import <Foundation/Foundation.h>

#include "Firestore/core/src/firebase/firestore/model/database_id.h"
#include "Firestore/core/src/firebase/firestore/model/document_key.h"

namespace model = firebase::firestore::model;

NS_ASSUME_NONNULL_BEGIN

/**
 * An internal representation of FIRDocumentReference, representing a key in a specific database.
 * This is necessary because keys assume a database from context (usually the current one).
 * FSTDocumentKeyReference binds a key to a specific databaseID.
 *
 * TODO(b/64160088): Make DocumentKey aware of the specific databaseID it is tied to.
 */
@interface FSTDocumentKeyReference : NSObject

- (instancetype)init NS_UNAVAILABLE;

- (instancetype)initWithKey:(model::DocumentKey)key
                 databaseID:(const model::DatabaseId *)databaseID NS_DESIGNATED_INITIALIZER;

- (const model::DocumentKey &)key;

// Does not own the DatabaseId instance.
@property(nonatomic, assign, readonly) const model::DatabaseId *databaseID;

@end

NS_ASSUME_NONNULL_END
