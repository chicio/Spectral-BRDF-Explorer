//
//  ObjListViewController.m
//  OpenGL
//
//  Created by Fabrizio Duroni on 31/05/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#import "GLViewController.h"
#import "ObjListViewController.h"

@interface ObjListViewController ()
@property (nonatomic, strong) NSArray *objNameList;
@end

@implementation ObjListViewController

- (void)viewDidLoad {
    
    //Load obj names.
    self.objNameList = @[
        @"cube",
        @"sphere",
        @"suzanne",
        @"bunny",
        @"buddha"
    ];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    
    return 5;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"ObjModelNameCell" forIndexPath:indexPath];
    
    //Setup cell.
    cell.textLabel.text = [self.objNameList objectAtIndex:indexPath.row];
    
    return cell;
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {

    NSIndexPath *indexPath = [self.tableView indexPathForSelectedRow];
    GLViewController *glViewController = [segue destinationViewController];
    NSString *modelName = [self.objNameList objectAtIndex:indexPath.row];
    
    //Set model name.
    [glViewController setModel:modelName];
}

@end
