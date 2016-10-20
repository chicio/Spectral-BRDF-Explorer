//
//  SceneListViewController.m
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 31/05/16.
//  
//

#include "SceneLoader.h"
#import "SceneListViewController.h"

@interface SceneListViewController ()
@property (nonatomic, strong) NSArray *objNameList;
@end

@implementation SceneListViewController

- (void)viewDidLoad {
    
    //Load obj names.
    self.objNameList = @[@"RGB Scene",
                         @"Spectral Scene"];
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
            SceneLoader::loadRGBScene();
            break;
    }
}

@end
