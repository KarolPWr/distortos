/**
 * \file
 * \brief DynamicRawFifoQueue class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/DynamicRawFifoQueue.hpp"

#include "distortos/internal/memory/storageDeleter.hpp"

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

DynamicRawFifoQueue::DynamicRawFifoQueue(const size_t elementSize, const size_t queueSize) :
		RawFifoQueue{{new uint8_t[elementSize * queueSize], internal::storageDeleter<uint8_t>}, elementSize, queueSize}
{

}

}	// namespace distortos
