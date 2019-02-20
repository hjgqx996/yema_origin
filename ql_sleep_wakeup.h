#ifndef __QL_SLEEP_H__
#define __QL_SLEEP_H__


typedef int Ql_handle;

/*
 * Request the handle to enable/disable sleep for specified module
 */
extern Ql_handle QL_SLP_request_handle(void);

/*
 * Release the handle
 */
extern void QL_SLP_release_handle(Ql_handle handle);


/*
 * Enable/disable sleep function of the module specified by the handle, allowsleep=1 means allowed to sleep, others not allowed
 */
extern void QL_SLP_set_allow(Ql_handle handle, int allowsleep);

/*
 * Register wakeup notify function
 */
extern void QL_SLP_register_wakeup_notify(int (*onWakeup)(char* wakeup_src, void* reserved));

#endif  //__QL_SLEEP_H__