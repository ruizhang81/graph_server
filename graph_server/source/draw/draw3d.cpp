#pragma once
#include "../data_structure/z_include.h"

#ifndef WEBASSEMBLY
    #ifdef MAC_OS

        void draw3d(requestParam &requestParamObj){


        cv::viz::Viz3d window("Visual Odometry");
        //    window.showWidget("Coordinate Widget", viz::WCoordinateSystem());
        window.setBackgroundColor(cv::viz::Color::white());


        for (edge *edgeItem:requestParamObj.graphObj.edges)
        {
            nodeEntity *sourceNode = edgeItem->mSource;
            nodeEntity *targetNode = edgeItem->mTarget;
            Point3d point_begin(sourceNode->mCenterX,
                                sourceNode->mCenterY,
                                sourceNode->mCenterZ);
            Point3d point_end(targetNode->mCenterX,
                              targetNode->mCenterY,
                              targetNode->mCenterZ);
            viz::WLine line3(point_begin, point_end, viz::Color::green());
            line3.setRenderingProperty(viz::LINE_WIDTH, 1.0);
            
            string buf = "edge";
            buf = buf + edgeItem->id;
            window.showWidget(buf.c_str(), line3);
        }

        for(nodeEntity *node:requestParamObj.graphObj.nodes){
            //cout << node->id << " " << node->mCenterX << " " << node->mCenterY << " " << node->mCenterZ << endl;
            viz::WSphere sphere_widget(Point3f(node->mCenterX,
                                               node->mCenterY,
                                               node->mCenterZ), 10, 10, viz::Color::blue());
            sphere_widget.setRenderingProperty(viz::LINE_WIDTH, 1.0);
            window.showWidget(node->id.c_str(), sphere_widget);
        }

        while(!window.wasStopped())
        {
            window.spinOnce(1, true);
        }
        }

    #endif
#endif
