/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#if !defined(DXVA2Manager_h_)
#define DXVA2Manager_h_

#include "WMF.h"
#include "nsAutoPtr.h"
#include "mozilla/Mutex.h"
#include "nsRect.h"

namespace mozilla {

namespace layers {
class Image;
class ImageContainer;
class KnowsCompositor;
}

class DXVA2Manager {
public:

  // Creates and initializes a DXVA2Manager. We can use DXVA2 via either
  // D3D9Ex or D3D11.
  static DXVA2Manager* CreateD3D9DXVA(layers::KnowsCompositor* aKnowsCompositor, nsACString& aFailureReason);
  static DXVA2Manager* CreateD3D11DXVA(layers::KnowsCompositor* aKnowsCompositor, nsACString& aFailureReason);

  // Returns a pointer to the D3D device manager responsible for managing the
  // device we're using for hardware accelerated video decoding. If we're using
  // D3D9Ex, this is an IDirect3DDeviceManager9. For D3D11 this is an
  // IMFDXGIDeviceManager. It is safe to call this on any thread.
  virtual IUnknown* GetDXVADeviceManager() = 0;

  // Creates an Image for the video frame stored in aVideoSample.
  virtual HRESULT CopyToImage(IMFSample* aVideoSample,
                              const nsIntRect& aRegion,
                              layers::Image** aOutImage) = 0;

  virtual HRESULT ConfigureForSize(uint32_t aWidth, uint32_t aHeight) { return S_OK; }

  virtual bool IsD3D11() { return false; }

  virtual ~DXVA2Manager();

  virtual bool SupportsConfig(IMFMediaType* aType, float aFramerate) = 0;

protected:
  Mutex mLock;
  DXVA2Manager();
};

} // namespace mozilla

#endif // DXVA2Manager_h_
