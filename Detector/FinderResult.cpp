//
//  FinderResult.cpp
//  QRcodeCPP
//
//  Created by 范志方 on 16/5/25.
//  Copyright © 2016年 范志方. All rights reserved.
//

#include "FinderResult.hpp"

FinderResult::FinderResult(vector<FinderPoint> pts) : bottomLeft(pts[0]), topLeft(pts[1]), topRight(pts[2]){
}

