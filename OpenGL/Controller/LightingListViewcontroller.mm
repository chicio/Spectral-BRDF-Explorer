//
//  LightingListViewcontroller.m
//  OpenGL
//
//  Created by Fabrizio Duroni on 02/06/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#include <vector>

#import "LightingListViewcontroller.h"
#include "Scene.hpp"

@interface LightingListViewcontroller ()
@property (nonatomic, strong) NSArray *lightingList;
@end

@implementation LightingListViewcontroller

- (void)viewDidLoad {
    
    //Load material names and data.
    self.lightingList = @[
        @"Blinn-Phong",
        @"Phong"
    ];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    
    return self.lightingList.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"LightingCell" forIndexPath:indexPath];
    
    //Setup cell.
    cell.textLabel.text = [self.lightingList objectAtIndex:indexPath.row];
    
    return cell;
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    
    NSIndexPath *indexPath = [self.tableView indexPathForSelectedRow];
    NSString *lightingName = [self.lightingList objectAtIndex:indexPath.row];
    
    //Parse lighting name to get lighting shaders name.
    lightingName = [lightingName stringByReplacingOccurrencesOfString:@"-"
                                                           withString:@""];
    
    lightingName = [lightingName stringByReplacingOccurrencesOfString:@" "
                                                           withString:@""];
    
    
    //Set lighting.
    Scene::instance().lighting = [lightingName UTF8String];
}

@end