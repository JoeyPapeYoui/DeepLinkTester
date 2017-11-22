//
//  DeepLinkCaller-C-Interface.h
//  ListViewAE
//
//  Created by Joseph Pape on 2017-01-25.
//
//

#ifndef DeepLinkCaller_C_Interface_h
#define DeepLinkCaller_C_Interface_h

class CYIString;

// This is the C "trampoline" function that will be used
// to invoke a specific Objective-C method FROM C++
void CallDeepLink (CYIString sURL);

#endif /* DeepLinkCaller_C_Interface_h */
