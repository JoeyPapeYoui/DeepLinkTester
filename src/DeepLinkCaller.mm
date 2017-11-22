//
//  CallDeepLink.m
//  ListViewAE
//
//  Created by Joseph Pape on 2017-01-25.
//
//

#import "DeepLinkCaller.h"
#import <UIKit/UIKit.h>
#include <utility/YiString.h>

@implementation DeepLinkCaller

// C "trampoline" function to invoke Objective-C method
void CallDeepLink (CYIString sURL)
{
    NSString* nsURL = [[NSString alloc] initWithUTF8String:sURL.AsUTF8()];
    
    // Call the Objective-C method using Objective-C syntax
    [DeepLinkCaller CallDeepLink:nsURL];
}

+ (void) CallDeepLink:(NSString *) urlString
{
    NSURL *url = [NSURL URLWithString:urlString];
    YI_UNUSED(url);
    [[UIApplication sharedApplication] openURL:url];
}

@end
