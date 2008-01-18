/*********************************************************************
 * Common set of tools (classes & functions)
 *
 * Dynamic library specific defines
 *
 * Copyright (C) 2003 Julien Coloos.  All rights reserved.
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech AS of Norway and appearing in the file
 * LICENSE.QPL included in the packaging of this file.
 *
 * This file may be distributed and/or modified under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.
 *
 * Licensees holding an other license may use this file in accordance with 
 * the Agreement provided with the Software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See http://www.opensource.org/licenses/qtpl.php for QPL licensing information.
 * See http://www.opensource.org/licenses/gpl-license.html for GPL licensing information.
 *
 * Contact cyrius@corecodec.org if any conditions of this licensing
 * are not clear to you.
 *
 ********************************************************************/

#ifndef __TOOLS_COMMON_DLL_H__
#define __TOOLS_COMMON_DLL_H__

/*
 * Provide the storage class specifier (extern for an .exe file, null
 * for DLL) and the __declspec specifier (dllimport for an .exe file,
 * dllexport for DLL).
 * You must define TOOLS_DLL_EXPORTS when compiling the DLL.
 * You can now use this header file in both the .exe file and DLL - a
 * much safer means of using common declarations than two different
 * header files.
 */

#ifdef TOOLS_DLL
#	ifdef TOOLS_DLL_EXPORTS
#		define TOOLS_DLL_API __declspec(dllexport)
#		define TOOLS_EXPIMP_TEMPLATE
#	else
#		define TOOLS_DLL_API __declspec(dllimport)
#		define TOOLS_EXPIMP_TEMPLATE extern
#	endif
#else
#	define TOOLS_DLL_API
#	ifdef TOOLS_DLL_EXPORTS
#		define TOOLS_EXPIMP_TEMPLATE
#	else
#		define TOOLS_EXPIMP_TEMPLATE extern
#	endif
#endif

#endif /* __TOOLS_COMMON_DLL_H__ */
