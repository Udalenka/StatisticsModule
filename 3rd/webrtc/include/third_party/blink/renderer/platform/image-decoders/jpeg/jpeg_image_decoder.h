/*
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_IMAGE_DECODERS_JPEG_JPEG_IMAGE_DECODER_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_IMAGE_DECODERS_JPEG_JPEG_IMAGE_DECODER_H_

#include <memory>

#include "third_party/blink/renderer/platform/image-decoders/image_decoder.h"

namespace blink {

class JPEGImageReader;

class PLATFORM_EXPORT JPEGImageDecoder final : public ImageDecoder {
 public:
  JPEGImageDecoder(AlphaOption,
                   const ColorBehavior&,
                   wtf_size_t max_decoded_bytes,
                   wtf_size_t offset = 0);
  JPEGImageDecoder(const JPEGImageDecoder&) = delete;
  JPEGImageDecoder& operator=(const JPEGImageDecoder&) = delete;
  ~JPEGImageDecoder() override;

  // ImageDecoder:
  String FilenameExtension() const override { return "jpg"; }
  void OnSetData(SegmentReader* data) override;
  IntSize DecodedSize() const override { return decoded_size_; }
  bool SetSize(unsigned width, unsigned height) override;
  cc::YUVSubsampling GetYUVSubsampling() const override;
  IntSize DecodedYUVSize(cc::YUVIndex) const override;
  wtf_size_t DecodedYUVWidthBytes(cc::YUVIndex) const override;
  void DecodeToYUV() override;
  SkYUVColorSpace GetYUVColorSpace() const override;
  Vector<SkISize> GetSupportedDecodeSizes() const override;

  bool HasImagePlanes() const { return image_planes_.get(); }

  bool OutputScanlines();
  unsigned DesiredScaleNumerator() const;
  bool ShouldGenerateAllSizes() const;
  void Complete();

  void SetOrientation(ImageOrientation orientation) {
    orientation_ = orientation;
  }

  void SetDensityCorrectedSize(const IntSize& size) {
    density_corrected_size_ = size;
  }
  void SetDecodedSize(unsigned width, unsigned height);

  void SetSupportedDecodeSizes(Vector<SkISize> sizes);

  enum class DecodingMode {
    // Stop decoding after calculating the image size and parsing the header.
    kDecodeHeader,
    // Assumes that YUV decoding is possible. Eg. image planes are set and
    // CanDecodeToYUV is true.
    kDecodeToYuv,
    // For images that can be decoded as YUV, the caller may request
    // non-YUV decoding anyway. Eg. when bitmap backing is needed.
    kDecodeToBitmap,
  };

 private:
  // ImageDecoder:
  void DecodeSize() override { Decode(DecodingMode::kDecodeHeader); }
  void Decode(wtf_size_t) override {
    // Use DecodeToYUV for YUV decoding.
    Decode(DecodingMode::kDecodeToBitmap);
  }
  cc::ImageHeaderMetadata MakeMetadataForDecodeAcceleration() const override;

  // Attempts to calculate the coded size of the JPEG image. Returns a zero
  // initialized gfx::Size upon failure.
  gfx::Size GetImageCodedSize() const;

  // Decodes the image. If decoding fails but there is no more
  // data coming, sets the "decode failure" flag.
  void Decode(DecodingMode decoding_mode);

  std::unique_ptr<JPEGImageReader> reader_;
  const wtf_size_t offset_;
  IntSize decoded_size_;
  Vector<SkISize> supported_decode_sizes_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_PLATFORM_IMAGE_DECODERS_JPEG_JPEG_IMAGE_DECODER_H_
