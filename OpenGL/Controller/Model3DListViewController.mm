//
//  ObjListViewController.m
//  OpenGL
//
//  Created by Fabrizio Duroni on 31/05/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#import "Model3DListViewController.h"

#include "Scene.hpp"

@interface Model3DListViewController ()
@property (nonatomic, strong) NSArray *objNameList;
@end

@implementation Model3DListViewController

- (void)viewDidLoad {
    
    //Load obj names.
    self.objNameList = @[
        @"Cube",
        @"Sphere",
        @"Suzanne",
        @"Bunny",
        @"Happy Buddha"
    ];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    
    return self.objNameList.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Model3DNameCell" forIndexPath:indexPath];
    
    //Setup cell.
    cell.textLabel.text = [self.objNameList objectAtIndex:indexPath.row];
    
    return cell;
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {

    NSIndexPath *indexPath = [self.tableView indexPathForSelectedRow];

    NSString *modelName = [self.objNameList objectAtIndex:indexPath.row];
    
    //Parse model name.
    NSString *modelNameParsed = [modelName stringByReplacingOccurrencesOfString:@" "
                                                                     withString:@""];
    
    const char* modelFilePath = [[[NSBundle mainBundle] pathForResource:modelNameParsed
                                                                 ofType:@"obj"]
                                 cStringUsingEncoding:NSUTF8StringEncoding];

    
    //Create model.
    Model3D model3D(modelFilePath, [modelName UTF8String]);
    
    //Set model for the current scene.
    Scene::instance().model = model3D;
}

@end
