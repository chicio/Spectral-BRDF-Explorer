//
//  ObjListViewController.m
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 31/05/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
        @"Dragon",
        @"Bunny",
        @"Happy Buddha",
        @"Lucy"
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

    //Model path.
    NSIndexPath *indexPath = [self.tableView indexPathForSelectedRow];
    NSString *modelName = [self.objNameList objectAtIndex:indexPath.row];
    NSString *modelNameParsed = [modelName stringByReplacingOccurrencesOfString:@" "
                                                                     withString:@""];
    
    const char* modelFilePath = [[[NSBundle mainBundle] pathForResource:modelNameParsed
                                                                 ofType:@"obj"]
                                 cStringUsingEncoding:NSUTF8StringEncoding];
    
    ///Cornell box path.
    const char* cornellBoxModelFilePath = [[[NSBundle mainBundle] pathForResource:@"CornellBox"
                                                                           ofType:@"obj"]
                                           cStringUsingEncoding:NSUTF8StringEncoding];

//    const char* planeFilePath = [[[NSBundle mainBundle] pathForResource:@"Plane"
//                                                                 ofType:@"obj"]
//                                                   cStringUsingEncoding:NSUTF8StringEncoding];
    
    //Create model and Cornell Box.
    Model3D model3D(modelFilePath, [modelName UTF8String]);
    model3D._modelMatrix = glm::translate(glm::mat4(), glm::vec3(0.0, 0.0f, -12.0f));
    Scene::instance().models.push_back(model3D);

    //Cornell Box.
    Model3D cornellBoxBottom3D(cornellBoxModelFilePath, "Cornell Box");
    cornellBoxBottom3D._modelMatrix = glm::translate(cornellBoxBottom3D._modelMatrix, glm::vec3(0.0, 0.0f, -12.0f));
    cornellBoxBottom3D.setMaterial(Material::createMatteMaterial());
    Scene::instance().models.push_back(cornellBoxBottom3D);
}

@end
