/*
 * StorageStates.h
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#ifndef INCLUDES_APRA_MODELS_STORAGESTATES_H_
#define INCLUDES_APRA_MODELS_STORAGESTATES_H_

namespace apra
{
enum STORAGE_STATE
{
	STORAGE_INSERTED, STORAGE_INSERTED_UNMOUNTED, STORAGE_MOUNTED, STORAGE_UNSAFE_EJECT, STORAGE_SAFE_EJECT
};
}

#endif /* INCLUDES_MODELS_STORAGESTATES_H_ */
