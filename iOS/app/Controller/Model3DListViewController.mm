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
                         @"RGB Scene",
                         @"Spectral Scene"
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
    }
}

-(void)scene1 {
    
    Scene::instance().clearScene();
    
    Model3D model3D0("Dragon.obj", "Dragon");
    model3D0._modelMatrix = glm::scale(glm::mat4(), glm::vec3(1.10f, 1.10f, 1.10f));
    model3D0._modelMatrix = glm::translate(model3D0._modelMatrix, glm::vec3(-5.0, 0.0f, -8.0f));
    model3D0._modelMatrix = glm::rotate(model3D0._modelMatrix, glm::radians(30.0f), glm::vec3(0.0, 1.0f, 0.0f));
    model3D0.setMaterial(Material::createRubyMaterial());
    model3D0.lighting = "OrenNayar";
    Scene::instance().models.push_back(model3D0);
    
    Model3D model3D1("HappyBuddha.obj", "Happy Buddha");
    model3D1._modelMatrix = glm::translate(glm::mat4(), glm::vec3(-2.0f, 0.0f, -11.0f));
    model3D1.setMaterial(Material::createBronzeMaterial());
    model3D1.lighting = "CookTorrance";
    Scene::instance().models.push_back(model3D1);
    
    Model3D model3D2("ThaiStatue.obj", "Thai Statue");
    model3D2._modelMatrix = glm::translate(glm::mat4(), glm::vec3(2.0f, 0.0f, -13.0f));
    model3D2.setMaterial(Material::createMatteMaterial());
    model3D2.lighting = "Lambertian";
    Scene::instance().models.push_back(model3D2);
    
    Model3D model3D3("Lucy.obj", "Lucy");
    model3D3._modelMatrix = glm::translate(glm::mat4(), glm::vec3(5.0f, 0.0f, -11.0f));
    model3D3.setMaterial(Material::createJadeMaterial());
    model3D3.setTextureName("Lucy-texture.png");
    model3D3.lighting = "Phong";
    Scene::instance().models.push_back(model3D3);
    
    Model3D model3D4("Bunny.obj", "Bunny");
    model3D4._modelMatrix = glm::translate(glm::mat4(), glm::vec3(0.5f, 0.0f, -7.5f));
    model3D4.setMaterial(Material::createOrangeMaterial());
    model3D4.lighting = "BlinnPhong";
    Scene::instance().models.push_back(model3D4);
    
    Model3D cornellBoxBottom3D("Floor.obj", "Cornell Box");
    cornellBoxBottom3D._modelMatrix = glm::scale(cornellBoxBottom3D._modelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
    cornellBoxBottom3D._modelMatrix = glm::translate(cornellBoxBottom3D._modelMatrix, glm::vec3(0.0, 0.0f, -6.0f));
    cornellBoxBottom3D.setMaterial(Material::createMatteMaterial());
    cornellBoxBottom3D.lighting = "Lambertian";
    Scene::instance().models.push_back(cornellBoxBottom3D);
    
    Model3D skybox("Cube.obj", "Skybox");
    skybox._modelMatrix = glm::scale(glm::mat4(), glm::vec3(60.0, 60.0f, 60.0f));;
    skybox.setMaterial(Material::createMatteMaterial());
    skybox.lighting = "Lambertian";
    Scene::instance().skybox = skybox;
    
    Scene::instance().nearPlane = 0.1f;
    Scene::instance().farPlane = 200.0f;
    Scene::instance().sceneCenter = glm::vec3(0.0f, 0.0f, -17.0f);
    Scene::instance().lightDirection = glm::vec3(3.0f, 3.0f, 3.0f);
}

@end
