/**
 * SECTION:gcoroutine-version
 * @title: Versioning information
 * @short_description: Detemining the version of Gcoroutine in use
 *
 * Gcoroutine provides symbols to know the version of the library at compile
 * time.
 */

#ifndef __GCOROUTINE_VERSION_H__
#define __GCOROUTINE_VERSION_H__

#if !defined(GCOROUTINE_H_INSIDE) && !defined(GCOROUTINE_COMPILATION)
#error "Only gcoroutine.h can be included directly."
#endif

/**
 * GCOROUTINE_MAJOR_VERSION:
 *
 * Evaluates to the major version number of the library version,
 * e.g. 1 in 1.2.3.
 *
 * Since: 1.0
 */
#define GCOROUTINE_MAJOR_VERSION          (UNKNOWN)

/**
 * GCOROUTINE_MINOR_VERSION:
 *
 * Evaluates to the minor version number of the library version,
 * e.g. 2 in 1.2.3.
 *
 * Since: 1.0
 */
#define GCOROUTINE_MINOR_VERSION          (UNKNOWN)

/**
 * GCOROUTINE_MICRO_VERSION:
 *
 * Evaluates to the micro version number of the library version,
 * e.g. 3 in 1.2.3.
 *
 * Since: 1.0
 */
#define GCOROUTINE_MICRO_VERSION          (UNKNOWN)

#endif /* __GCOROUTINE_VERSION_H__ */
