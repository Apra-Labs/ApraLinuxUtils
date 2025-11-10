/*
 * ThreadType.h
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#ifndef INCLUDES_APRA_CONSTANTS_THREADTYPE_H_
#define INCLUDES_APRA_CONSTANTS_THREADTYPE_H_

namespace apra
{
enum THREAD_TYPE
{
	FREERUNNING, MESSAGE_AND_FREERUNNING, ONLY_MESSAGE
};
}

#endif /* INCLUDES_APRA_CONSTANTS_THREADTYPE_H_ */
