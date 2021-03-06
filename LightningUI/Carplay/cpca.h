#ifndef _CPCA_H_
#define _CPCA_H_

#define FILE_DEVICE_CPCA          0x00000467

#define IOCTL_CODE_BASE           3000


// define the IOCTRL code passed to CPC_IOControl
#define IOCTL_CPCA_SET_CERT \
	CTL_CODE(FILE_DEVICE_CPCA, IOCTL_CODE_BASE + 1, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_CPCA_GET_CERT \
	CTL_CODE(FILE_DEVICE_CPCA, IOCTL_CODE_BASE + 2, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_CPCA_DELETE_CERT \
	CTL_CODE(FILE_DEVICE_CPCA, IOCTL_CODE_BASE + 3, METHOD_BUFFERED, FILE_ANY_ACCESS)

#endif


