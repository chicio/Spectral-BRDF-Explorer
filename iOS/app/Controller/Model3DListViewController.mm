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
                         @"Scene 1",
                         @"Scene 2",
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
    
    NSIndexPath* path = [self.tableView indexPathForSelectedRow];
    
    switch(path.row) {
        case 0:
            [self scene1];
            break;
        case 1:
            [self scene2];
            break;
    }
}

- (void)scene1 {
    
    //Create model and Cornell Box.
    Model3D model3D("Sphere.obj", "Sphere");
    model3D._modelMatrix = glm::translate(glm::mat4(), glm::vec3(0.0, 0.0f, -12.0f));
    model3D.setMaterial(Material::createBronzeMaterial());
    model3D.lighting = "BlinnPhong";
    
    if(model3D.modelData().hasTexture()) {
        
        NSString *textureName = [NSString stringWithFormat:@"%s-texture", model3D.getName().c_str()];
        const char* textureFilePath = [[[NSBundle mainBundle] pathForResource:textureName
                                                                       ofType:@"png"]
                                       cStringUsingEncoding:NSUTF8StringEncoding];
        model3D.modelData().loadTexture(textureFilePath);
    }
    
    Scene::instance().models.push_back(model3D);
    
    //Cornell Box.
    Model3D cornellBoxBottom3D("CornellBox.obj", "Cornell Box");
    cornellBoxBottom3D._modelMatrix = glm::translate(cornellBoxBottom3D._modelMatrix, glm::vec3(0.0, 0.0f, -12.0f));
    cornellBoxBottom3D.setMaterial(Material::createMatteMaterial());
    cornellBoxBottom3D.lighting = "Phong";
    Scene::instance().models.push_back(cornellBoxBottom3D);
}

- (void)scene2 {
    
    //Create model and Cornell Box.
    Model3D model3D("Lucy.obj", "Sphere");
    model3D._modelMatrix = glm::translate(glm::mat4(), glm::vec3(0.0, 0.0f, -12.0f));
    model3D.setMaterial(Material::createMatteMaterial());
    model3D.lighting = "OrenNayar";
    
    if(model3D.modelData().hasTexture()) {
        
        model3D.modelData().loadTexture("Lucy-texture.png");
    }
    
    Scene::instance().models.push_back(model3D);
    
    //Cornell Box.
    Model3D cornellBoxBottom3D("CornellBox.obj", "Cornell Box");
    cornellBoxBottom3D._modelMatrix = glm::translate(cornellBoxBottom3D._modelMatrix, glm::vec3(0.0, 0.0f, -12.0f));
    cornellBoxBottom3D.setMaterial(Material::createMatteMaterial());
    cornellBoxBottom3D.lighting = "Phong";
    Scene::instance().models.push_back(cornellBoxBottom3D);
}

@end
