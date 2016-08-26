//
//  MaterialListViewController.m
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/06/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#include <vector>

#import "MaterialListViewController.h"
#include "Scene.hpp"

@interface MaterialListViewController () {

    std::vector<Material> _materialList;
}

@property (nonatomic, strong) NSArray *materialNameList;
@end

@implementation MaterialListViewController

- (void)viewDidLoad {
    
    //Load material names and data.
    self.materialNameList = @[
        @"Ruby",
        @"Bronze",
        @"Jade",
        @"Orange",
        @"Matte"
    ];

    _materialList = {
        Material::createRubyMaterial(),
        Material::createBronzeMaterial(),
        Material::createJadeMaterial(),
        Material::createOrangeMaterial(),
        Material::createMatteMaterial()
    };
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    
    return self.materialNameList.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"MaterialCell" forIndexPath:indexPath];
    
    //Setup cell.
    cell.textLabel.text = [self.materialNameList objectAtIndex:indexPath.row];
    
    return cell;
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    
    NSIndexPath *indexPath = [self.tableView indexPathForSelectedRow];
    
    //Set materials for the current scene model and cornell box.
    Scene::instance().model.setMaterial(_materialList[indexPath.row]);
    Scene::instance().cornellBoxModel.setMaterial(_materialList[4]);
}

@end
