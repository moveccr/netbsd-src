#pragma once

#include <memory.h>
#include "audiovar.h"
#include "auformat.h"

/*
* ***** audio_ring *****
*/

/* ring の top フレームのポインタを求めます。 */
#define RING_TOP(type, ringptr) (((type*)(ringptr)->sample) + (ringptr)->top * (ringptr)->fmt.channels)

/* ring の bottom (= top + count、すなわち、最終有効フレームの次) フレームのポインタを求めます。 */
#define RING_BOT(type, ringptr) (((type*)(ringptr)->sample) + audio_ring_bottom(ringptr) * (ringptr)->fmt.channels)

/* ring の frame 位置のポインタを求めます。 */
#define RING_PTR(type, ringptr, frame) (((type*)(ringptr)->sample) + audio_ring_round((ringptr), (ringptr)->top + frame) * (ringptr)->fmt.channels)

/* stride=24 用 */

/* ring の top フレームのポインタを求めます。 */
#define RING_TOP_UINT8(ringptr) (((uint8_t*)(ringptr)->sample) + (ringptr)->top * (ringptr)->fmt.channels * (ringptr)->fmt.stride / 8)

/* ring の bottom (= top + count、すなわち、最終有効フレームの次) フレームのポインタを求めます。 */
#define RING_BOT_UINT8(ringptr) (((uint8_t*)(ringptr)->sample) + audio_ring_bottom(ringptr) * (ringptr)->fmt.channels * (ringptr)->fmt.stride / 8)

/* キャパシティをバイト単位で求めます。 */
#define RING_BYTELEN(ringptr) ((ringptr)->capacity * (ringptr)->fmt.channels * (ringptr)->fmt.stride / 8)

/* ring の バッファ終端を求めます。この位置へのアクセスは出来ません。 */
#define RING_END_PTR(type, ringptr) ((type*)(ringptr)->sample + (ringptr)->capacity * (ringptr)->fmt.channels)

static inline bool
is_valid_ring(const audio_ring_t *ring)
{
	if (ring == NULL) return false;
	if (!is_valid_format(&ring->fmt)) return false;
	if (ring->capacity < 0) return false;
	if (ring->capacity > INT_MAX / 2) return false;
	if (ring->count < 0) return false;
	if (ring->count > ring->capacity) return false;
	if (ring->capacity == 0) {
		if (ring->sample != NULL) return false;
	} else {
		if (ring->sample == NULL) return false;
		if (ring->top < 0) return false;
		if (ring->top >= ring->capacity) return false;
	}
	return true;
}

/*
* idx をラウンディングします。
* 加算方向で、加算量が ring->capacity 以下のケースのみサポートします。
*/
static inline int
audio_ring_round(audio_ring_t *ring, int idx)
{
	KASSERT(is_valid_ring(ring));
	KASSERT(idx >= 0);
	KASSERT(idx < ring->capacity * 2);

	return idx >= ring->capacity ? idx - ring->capacity : idx;
}

/*
 * ring->top から n 個取り出したことにします。
 */
static inline void
audio_ring_tookfromtop(audio_ring_t *ring, int n)
{
	KASSERT(is_valid_ring(ring));
	KASSERT(n >= 0);
	KASSERT(ring->count >= n);

	ring->top = audio_ring_round(ring, ring->top + n);
	ring->count -= n;
	if (ring->count == 0) ring->top = 0;
}

/*
 * ring bottom に n 個付け足したことにします。
 */
static inline void
audio_ring_appended(audio_ring_t *ring, int n)
{
	KASSERT(is_valid_ring(ring));
	KASSERT(n >= 0);
	KASSERT(ring->count + n <= ring->capacity);

	ring->count += n;
}

/*
* ring の bottom 位置(top+count位置) を返します。この位置は、最終有効フレームの次のフレーム位置に相当します。
*/
static inline int
audio_ring_bottom(audio_ring_t *ring)
{
	return audio_ring_round(ring, ring->top + ring->count);
}

/*
* ring->top の位置からの有効フレームにアクセスしようとするとき、
* ラウンディングせずにアクセス出来る個数を返します。
*/
static inline int
audio_ring_unround_count(const audio_ring_t *ring)
{
	KASSERT(is_valid_ring(ring));

	return ring->top + ring->count <= ring->capacity ? ring->count : ring->capacity - ring->top;
}

/*
* audio_ring_bottom の位置から空きフレームにアクセスしようとするとき、
* ラウンディングせずにアクセス出来る、空きフレームの個数を返します。
*/
static inline int
audio_ring_unround_free_count(const audio_ring_t *ring)
{
	KASSERT(is_valid_ring(ring));

	/* ring の unround 終端まで使用されているときは、開始位置はラウンディング後なので < が条件 */
	if (ring->top + ring->count < ring->capacity) {
		return ring->capacity - (ring->top + ring->count);
	} else {
		return ring->capacity - ring->count;
	}
}

static inline void
audio_ring_concat(audio_ring_t *dst, audio_ring_t *src, int count)
{
	KASSERT(is_valid_ring(dst));
	KASSERT(is_valid_ring(src));
	KASSERT(count >= 0);
	KASSERT(dst->fmt.channels == src->fmt.channels);
	KASSERT(dst->fmt.stride == src->fmt.stride);

	count = min(count, src->count);
	count = min(count, dst->capacity - dst->count);

	while (count > 0) {
		int slice = count;
		slice = audio_ring_unround_count(src);
		slice = min(slice, audio_ring_unround_free_count(dst));
		memcpy(RING_BOT_UINT8(dst), RING_TOP_UINT8(src), slice * dst->fmt.channels * dst->fmt.stride / 8);
		audio_ring_appended(dst, slice);
		audio_ring_tookfromtop(src, slice);
		count -= slice;
	}
}

