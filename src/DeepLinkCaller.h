#import <Foundation/Foundation.h>
#import "DeepLinkCaller-C-Interface.h"

@interface DeepLinkCaller : NSObject {}

+ (void) CallDeepLink:(NSString *) urlString;

@end
