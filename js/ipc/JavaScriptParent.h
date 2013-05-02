
/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * vim: set ts=4 sw=4 et tw=80:
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_jsipc_JavaScriptParent__
#define mozilla_jsipc_JavaScriptParent__

#include "mozilla/jsipc/PJavaScriptParent.h"
#include "nsIJavaScriptParent.h"
#include "JavaScriptShared.h"
#include "jsclass.h"

namespace mozilla {
namespace jsipc {

class JavaScriptParent
  : public PJavaScriptParent,
    public JavaScriptShared
{
  public:
    JavaScriptParent();

    bool init();

  public:
    bool has(JSContext *cx, JS::HandleObject proxy, JS::HandleId id, bool *bp);
    bool hasOwn(JSContext *cx, JS::HandleObject proxy, JS::HandleId id, bool *bp);
    bool get(JSContext *cx, JS::HandleObject proxy, JS::HandleObject receiver,
             JS::HandleId id, JS::MutableHandleValue vp);
    bool set(JSContext *cx, JS::HandleObject proxy, JS::HandleObject receiver,
             JS::HandleId id, bool strict, JS::MutableHandleValue vp);
    bool call(JSContext *cx, JS::HandleObject proxy, const JS::CallArgs &args);

    JSObject *Unwrap(JSContext *cx, ObjectId objId) {
        JSAutoRequest request(cx);
        return unwrap(cx, objId);
    }

    void GetUtils(nsIJavaScriptParent **parent);
    void DecRef();
    void IncRef();
    void DestroyFromContent();

    void drop(JSObject *obj);

    static bool IsCPOW(JSObject *obj);
    static nsresult InstanceOf(JSObject *obj, const nsID *id, bool *bp);

    nsresult instanceOf(JSObject *obj, const nsID *id, bool *bp);
    bool getPropertyDescriptor(JSContext *cx, JS::HandleObject proxy, JS::HandleId id,
                               JSPropertyDescriptor *desc, unsigned flags);
    bool objectClassIs(JSContext *cx, JS::HandleObject obj, js::ESClassValue classValue);
    bool preventExtensions(JSContext *cx, JS::HandleObject proxy);
    bool isExtensible(JSObject *proxy);

  protected:
    JSObject *unwrap(JSContext *cx, ObjectId objId);

  private:
    bool makeId(JSContext *cx, JSObject *obj, ObjectId *idp);
    ObjectId IdOf(JSObject *obj);

    // Catastrophic IPC failure.
    JSBool ipcfail(JSContext *cx);

    // Check whether a return status is okay, and if not, propagate its error.
    bool ok(JSContext *cx, const ReturnStatus &status);

  private:
    nsCOMPtr<nsIJavaScriptParent> utils_;
    uint64_t refcount_;
    bool inactive_;
};

} // jsipc
} // mozilla

#endif // mozilla_jsipc_JavaScriptWrapper_h__
