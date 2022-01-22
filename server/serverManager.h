#ifndef __SERVER_MANAGER_H__
#define __SERVER_MANAGER_H__

typedef struct Application Application;

Application* CreateServerApplication ();

void DestroyServerApplication (Application* _application);

#endif
