#include "module.h"

void rotate_(void *begin, void *middle, void *end, usize size) {
	u8 *begin_  = begin;
	u8 *middle_ = middle;
	u8 *end_    = end;

	for (; begin_ < middle_ && middle_ < end_; ) {
		if (middle_ - begin_ <= end_ - middle_) {
			u8 *low_cursor = begin_;
			u8 *high_cursor = end_ - (middle_ - begin_);

			for (; high_cursor < end_;) {
				swap(low_cursor, high_cursor, size);
				low_cursor += size;
				high_cursor += size;
			}

			end_ -= middle_ - begin_;
		} else {
			u8 *low_cursor = begin_;
			u8 *high_cursor = middle_;

			for (; high_cursor < end_;) {
				swap(low_cursor, high_cursor, size);
				low_cursor += size;
				high_cursor += size;
			}

			begin_ = low_cursor;
		}
	}
}
