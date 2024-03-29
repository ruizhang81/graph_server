sigma.utils.adjustSiblingsOrder
    = function(parentNode, edges) {

    if(!parentNode || !parentNode._wt_children){
        console.log(arguments.callee
            , 'parentNode is null or parentNode._wt_children is not existed');
        return;
    }

    var nodes = parentNode._wt_children
        , edges = edges || []
        , visitedNodes = {}
        , node
        , index
        , len = nodes.length
        , j
        , adjacentNodes
        , flagLeft
        , flagRight
        ;

    while((index = _hasMoreIndex()) >= 0){
        node = nodes[index];
        visitedNodes[node.id] = 1;
        adjacentNodes = _getAdjacentNodes(node, nodes);
        flagLeft = 0;
        flagRight = 0;
        if(adjacentNodes && adjacentNodes.length){

            if(index>0 && adjacentNodes.indexOf(nodes[index-1]) < 0){
                for(j=index-1; j>=0; j--){
                    if(adjacentNodes.indexOf(nodes[j]) >= 0){
                        nodes.splice(index, 1);
                        nodes.splice(j, 0, node);
                        flagLeft = 1;
                        break;
                    }
                }
            } 

            if(flagLeft){
                continue;
            }

            if(index<len - 1 && adjacentNodes.indexOf(nodes[index+1]) < 0){
                for(j=index+1; j<len; j++){
                    if(adjacentNodes.indexOf(nodes[j]) >= 0){
                        nodes.splice(j-1, 0, node);
                        nodes.splice(index, 1);
                        break;
                    }
                }
            } 

        }
    }

    function _hasMoreIndex(){
        for(var i=0; i<len; i++){
            var _node = nodes[i];
            if(!visitedNodes[_node.id]){
                return i;            
            }
        }
        return -1;
    }

    function _getAdjacentNodes(node) {
        var retNodes = [];
        edges.forEach(function(edge){
            var adjNodeId, adjNode;

            if(edge.source == node.id){
                adjNodeId = edge.target;
            }

            if(edge.target == node.id){
                adjNodeId = edge.source;
            }

            if(!adjNodeId){
                return;
            }

            if(adjNodeId != node.id){
                if(nodes.map(function(node){return node.id;})
                    .indexOf(adjNodeId) >= 0){
                    adjNode = sigma.utils.getNodeById(nodes, adjNodeId);
                    retNodes.push(adjNode);
                }
            }
        });
        return retNodes;
    }

};   
sigma.utils.adjustSiblingsOrder2
    = function( parentNode, options ) {
    if ( !parentNode ) {
        throw new Error('sigma.utils.adjustSiblingsOrder2: no parentNode');
    }

    var opt = options || {}
        , siblings = parentNode._wt_children
        , siblingGroups
        , newSiblings = []
        ;

    siblingGroups = _getGroups( siblings );
    siblingGroups.sort( function( a, b ) {
        return a.weight - b.weight;
    } );
    _layoutGroups( siblingGroups );
    parentNode._wt_children = newSiblings;
    return { 
        siblingGroups: siblingGroups 
        , newSiblings: newSiblings
    };

    function _layoutGroups( groups ) {
        groups.forEach( function( group ) {
            var root = group.root
                , visitedNodes = {}
                , groupNodes = []
                ;

            _travel( root );
            if ( group.direction == 'rtl' ) {
                groupNodes.reverse();
            }
            newSiblings = newSiblings.concat( groupNodes );

            // pre-order depth travel
            function _travel( node ) {
                // must check first if node has been visited
                if ( visitedNodes[ node.id ] ) {
                    return;
                }
                visitedNodes[ node.id ] = 1;
                groupNodes.push( node );

                var leNodes = node._wt_lenodes 
                    // may have duplicated items in `children` array
                    , children = leNodes.di_brothers.concat( leNodes.indi_brothers )
                    ;

                _uniq( children );
                // drop out visited nodes
                for ( var i = children.length - 1; i >= 0; i-- ) {
                    if ( visitedNodes[ children[ i ].id ] ) {
                        children.splice( i, 1 );
                    }
                }

                children.forEach( function( child ) {
                    _travel( child );
                } );
            }
        } );
    }

    // divide into groups or trees
    function _getGroups( siblings ) {
        var groups = []
            , group
            , visitedNodes = {} 
            , node
            , info
            ;

        while ( ( node = _hasMore() ) ) {
            info = _depthTravel( node );
            group = _getRoot( info );
            if ( !group.root ) {
                group = { root: node, direction: 'ltr', weight: 4 };
            }
            groups.push( group );
        }
        return groups;

        function _getRoot( info ) {
            var root
                , direction = 'ltr'
                , weight = 4
                ;

            if ( ( root = info.hasLeftDiExBrother[ 0 ] ) ) {
                weight = 1;
            }
            else if ( ( root = info.hasRightDiExBrother[ 0 ] ) ) {
                direction = 'rtl';
                weight = 10;
            }
            else if ( ( root = info.hasLeftInDiExBrother[ 0 ] ) ) {
                weight = 2;
            }
            else if ( ( root = info.hasRightInDiExBrother[ 0 ] ) ) {
                direction = 'rtl';
                weight = 9;
            }
            else if ( ( root = info.hasLeftOtherExNode[ 0 ] ) ) {
                weight = 3;
            }
            else if ( ( root = info.hasRightOtherExNode[ 0 ] ) ) {
                direction = 'rtl';
                weight = 8;
            }
            else if ( ( root = info.leafOrSingle[ 0 ] ) ) {}
            else {
                root = null;
            }

            return {
                root: root
                , direction: direction
                , weight: weight 
            };
        }

        function _depthTravel( node ) {
            var leNodes = node._wt_lenodes 
                // may have duplicated items in `children` array
                , children = leNodes.di_brothers.concat( leNodes.indi_brothers )
                , di_ex_brothers = leNodes.di_ex_brothers
                , indi_ex_brothers = leNodes.indi_ex_brothers
                , other_ex_nodes = leNodes.other_ex_nodes
                , exNode
                , info = {
                    leafOrSingle: []
                    , hasLeftDiExBrother: []
                    , hasLeftInDiExBrother: []
                    , hasRightDiExBrother: []
                    , hasRightInDiExBrother: []
                    , hasLeftOtherExNode: []
                    , hasRightOtherExNode: []
                }
                ;

            _uniq( children );
            if ( children.length <= 1 ) {
                info.leafOrSingle.push( node );
            }

            for ( var i = 0; i < di_ex_brothers.length; i++ ) {
                exNode = di_ex_brothers[ i ];
                if ( typeof exNode.hier_x != 'undefined' ) {
                    info.hasLeftDiExBrother.push( node );
                    break;
                }
                else {
                    info.hasRightDiExBrother.push( node );
                    break;
                }
            }

            for ( var i = 0; i < indi_ex_brothers.length; i++ ) {
                exNode = indi_ex_brothers[ i ];
                if ( typeof exNode.hier_x != 'undefined' ) {
                    info.hasLeftInDiExBrother.push( node );
                    break;
                }
                else {
                    info.hasRightInDiExBrother.push( node );
                    break;
                }
            }

            for ( var i = 0; i < other_ex_nodes.length; i++ ) {
                exNode = other_ex_nodes[ i ];
                if ( typeof exNode.hier_x != 'undefined' ) {
                    info.hasLeftOtherExNode.push( node );
                    break;
                }
                else {
                    info.hasRightOtherExNode.push( node );
                    break;
                }
            }

            // drop out visited nodes
            for ( var i = children.length - 1; i >= 0; i-- ) {
                if ( visitedNodes[ children[ i ].id ] ) {
                    children.splice( i, 1 );
                }
            }

            visitedNodes[ node.id ] = 1;
            children.forEach( function( child ) {
                var _info = _depthTravel( child );
                for ( var i in info ) {
                    info[ i ] = info[ i ].concat( _info[ i ] );
                }
            } );

            return info;
        }

        function _hasMore() {
            for ( var i = 0; i < siblings.length; i++ ) {
                if ( !visitedNodes[ siblings[ i ].id ] ) {
                    return siblings[ i ];
                }
            }
            return null;
        }
    }

    function _uniq( nodesArray ) {
        var _visitedNodes = {}
            , _duplicatedIndexes = []
            , _node
            , _index
            ;

        for ( var i = 0; i < nodesArray.length; i++ ) {
            _node = nodesArray[ i ];
            if ( _visitedNodes[ _node.id ] ) {
                _duplicatedIndexes.push( i ); 
            }
            _visitedNodes[ _node.id ] = 1;
        }

        for ( i = _duplicatedIndexes.length - 1; i >= 0; i-- ) {
            _index = _duplicatedIndexes[ i ];
            nodesArray.splice( _index, 1 );
        }

        return nodesArray;
    }

};
sigma.utils.applyLayoutInstantly
    = function(nodes, options){
    var opt = options || {}
        , readPrefix = opt.readPrefix || ''
        , writePrefix = opt.writePrefix || ''
        , clearOld = opt.clearOld || 0
        ;

    if(!nodes || !nodes.length){
        return;
    }
    nodes.forEach(function(node){
        if(undefined !== node[readPrefix + 'x']){
            node[writePrefix + 'x'] = node[readPrefix + 'x'];
            node[writePrefix + 'y'] = node[readPrefix + 'y'];
            if(clearOld){
                delete node[readPrefix + 'x'];
                delete node[readPrefix + 'y'];
            }
        }
        else {
            node[writePrefix + 'x'] = node.x;
            node[writePrefix + 'y'] = node.y;
        }
    });
};
sigma.utils.avoidChildrenTravelThrough
    = function(parentNode, edges) {

    if(!parentNode || !parentNode._wt_children){
        console.log(arguments.callee
            , 'parentNode is null or parentNode._wt_children is not existed');
        return;
    }

    sigma.utils.avoidSameLevelTravelThrough(
        parentNode._wt_children
        , edges
    );
};  
sigma.utils.avoidSameLevelTravelThrough
    = function(nodesOfSameLevel, edges) {

    if(!nodesOfSameLevel){
        console.log(arguments.callee
            , 'nodesOfSameLevel is null');
        return;
    }

    var nodes = nodesOfSameLevel 
        , edges = edges || []
        , unit = unit || 1
        , node, fromNode, toNode
        , len = nodes.length
        , i, j, k
        ;

    nodes.forEach(function(node){
        delete node._wt_dy;
    });

    for(i=0; i<len; i++){
        fromNode = nodes[i];
        for(j=i+2; j<len; j++){
            toNode = nodes[j];
            if(_isConnected(fromNode, toNode)){
                for(k=i+1; k<j; k++){
                    node = nodes[k];     
                    // adjust once
                    if(node._wt_dy) {
                        continue;
                    }
                    node._wt_dy = ( k % 2 == 0 ? -1 : 1 );
                }
            }
        }
    } 

    function _isConnected(fromNode, toNode){
        for(var i=0; i<edges.length; i++){
            var edge = edges[i];
            if(edge.source == fromNode.id
                    && edge.target == toNode.id
                || edge.source == toNode.id
                    && edge.target == fromNode.id) {
                return 1;
            } 
        }
        return 0;
    }

};
sigma.utils.clearLocalAndExternalNodes
    = function( tree ) {
    if ( !tree ) {
        return;
    }

    _depthTravel( tree );

    function _depthTravel( tree ) {
        var children = tree._wt_children;

        delete tree._wt_lenodes;
        children.forEach( function( child ) {
            _depthTravel( child );
        } );
    }
};   
sigma.utils.clustersNodes
    = function(
        nodes
        , options
    ) {

    var opt = options || {}
        , root = opt.root || {x: 0, y: 0}
        , len 
        , numOfFirstLevel
        , clusterLevels
        , angleRange = opt.angleRange || 2 * Math.PI
        , radiusStep = opt.radiusStep || 100
        , angleInput = opt.angleInput || 0
        , angleStart = angleInput
        , writePrefix = opt.writePrefix || ''
        , readPrefix = opt.readPrefix || ''
        , randomRadius = opt.randomRadius
        , radius = 0, _r
        , _rx, _ry 
        , i, j, k
        , angles
        , PI = Math.PI
        , alen, mid, left, right
        , retObj = null 
        ;

    if(!nodes || !nodes.length){
        return retObj;
    }

    len = nodes.length; 
    numOfFirstLevel = opt.numOfFirstLevel 
        || sigma.utils.getNumOfFirstClusterLevel(len, 15, 1);
    clusterLevels = sigma.utils.getClusterLevels(numOfFirstLevel, len); 
    if ( len <= 1 ) {
        angleRange = 0;
    }

    if(angleRange < PI * 2){
        angleStart = ( 2 * PI - angleRange ) / 2 + angleInput;
    }

    retObj = {
        numOfFirstLevel: numOfFirstLevel
        , numOfNodes: len
        , clusterLevels: clusterLevels
        , angleInput: angleInput
        , angleRange: angleRange
        , angleStart: angleStart
        , radiusStep: radiusStep
    };

    angles = sigma.utils.interpolatesAngle(
        angleRange
        , numOfFirstLevel
        , clusterLevels 
        , angleStart
    );

    if(typeof root[writePrefix + 'x'] == 'undefined'){
        root[writePrefix + 'x'] = root.x;
        root[writePrefix + 'y'] = root.y;
    }

    k = 0;
    _rx = root[readPrefix + 'x'];
    _ry = root[readPrefix + 'y'];

    if(opt.centerFirst){
        for(i=0; i<angles.length && k < len; i++){
            radius += radiusStep;
            alen = angles[i].length;
            mid = Math.floor(alen / 2);
            for(j=0; j<=mid && k<len; j++, k++){
                _r = _getRadius(radius);
                nodes[k][writePrefix + 'x'] = _rx + _r * Math.cos(angles[i][mid - j]);
                nodes[k][writePrefix + 'y'] = _ry + _r * Math.sin(angles[i][mid - j]);

                _r = _getRadius(radius);
                if(k + 1 < len && mid + j < alen && j != 0){
                    k++;
                    nodes[k][writePrefix + 'x'] = _rx + _r * Math.cos(angles[i][mid + j]);
                    nodes[k][writePrefix + 'y'] = _ry + _r * Math.sin(angles[i][mid + j]);
                }
            }
        }
    }
    else if(opt.sidesFirst){
        for(i=0; i<angles.length && k < len; i++){
            radius += radiusStep;
            alen = angles[i].length;
            mid = Math.floor(alen / 2);
            for(j=0; j<=mid && k<len; j++, k++){
                _r = _getRadius(radius);
                left = j;
                right = alen - 1 - j;
                if(left > right) {
                    break;
                }

                nodes[k][writePrefix + 'x'] = _rx + _r * Math.cos(angles[i][left]);
                nodes[k][writePrefix + 'y'] = _ry + _r * Math.sin(angles[i][left]);

                if(k + 1 < len && left < right){
                    k++;
                    _r = _getRadius(radius);
                    nodes[k][writePrefix + 'x'] = _rx + _r * Math.cos(angles[i][right]);
                    nodes[k][writePrefix + 'y'] = _ry + _r * Math.sin(angles[i][right]);
                }
            }
        }
    }
    else {
        for(i=0; i<angles.length && k < len; i++){
            radius += radiusStep;
            alen = angles[i].length;
            for(j=0; j<alen && k<len; j++, k++){
                _r = _getRadius(radius);
                nodes[k][writePrefix + 'x'] = _rx + _r * Math.cos(angles[i][j]);
                nodes[k][writePrefix + 'y'] = _ry + _r * Math.sin(angles[i][j]);
            }
        }
    }

    return retObj;

    function _getRadius(radius){
        return radius 
            + ( randomRadius ? 0.5 * radiusStep * Math.random() : 0 );
    }

};
sigma.utils.clustersNodesNew
    = function(
        nodes
        , options
    ) {

    var opt = options || {}
        , root = opt.root || {x: 0, y: 0}
        , len 
        , radiusStep = opt.radiusStep || 100
        , nodeOccupiedSpace = opt.nodeOccupiedSpace || 10
        , writePrefix = opt.writePrefix || ''
        , readPrefix = opt.readPrefix || ''
        , randomRadius = opt.randomRadius
        , angleWellDistributed = opt.angleWellDistributed || 0
        , radiusWellDistributed = opt.radiusWellDistributed || 0
        , sortMethod = opt.sortMethod || 'default'

        , radius = 0, _r
        , _rx, _ry, _angle, _level
        , i, j, k
        , angles
        , PI = Math.PI
        , alen, mid, left, right
        , positions = []
        ;

    if(!nodes || !nodes.length){
        return;
    }

    len = nodes.length; 

    angles = sigma.utils.interpolatesAngleNew(
        len
        , radiusStep
        , nodeOccupiedSpace  
        , opt
    );

    // if empty angles, do nothing
    if ( !angles.length ) {
        return;
    }

    if(typeof root[writePrefix + 'x'] == 'undefined'){
        root[writePrefix + 'x'] = root.x;
        root[writePrefix + 'y'] = root.y;
    }

    k = 0;
    _rx = root[readPrefix + 'x'];
    _ry = root[readPrefix + 'y'];
    radius = radiusStep;

    // 2r, 3r, 5r, 7r, ...
    for(i=0; i<angles.length && k < len; i++){
        if ( i <= 1 || radiusWellDistributed ) {
            radius += radiusStep;
        }
        else {
            radius += 2 * radiusStep;
        }
        alen = angles[i].length;
        _level = i;
        for(j=0; j<alen && k<len; j++, k++){
            _r = _getRadius(radius);
            _angle = angles[i][j];
            positions.push( {
                x: _rx + _r * Math.cos(_angle)
                , y: _ry + _r * Math.sin(_angle)
                , angle: _angle
                , level: _level
            } );
        }
    }

    switch ( sortMethod ) {
        case 'level-angle':
            positions.sort( function( a, b ) { return b.angle - a.angle; } )
                .sort( function( a, b ) { return b.level - a.level; } );
            break;

        case 'angle-level':
            positions.sort( function( a, b ) { return b.level - a.level; } )
                .sort( function( a, b ) { return b.angle - a.angle; } );
            break;

        default:
            break;
    }

    for(i=0; i<len; i++){
        nodes[i][writePrefix + 'x'] = positions[i].x;
        nodes[i][writePrefix + 'y'] = positions[i].y;
    }

    function _getRadius(radius){
        return radius 
            + ( randomRadius ? 0.5 * radiusStep * Math.random() : 0 );
    }

};   
sigma.utils.computeCircleTreeRect
    = function(tree){

    var maxLevel = tree._wt_maxlevel
        , hasCircuit = tree._circuit ? 1 : 0
        , width = hasCircuit ? 2 * maxLevel : maxLevel
        ;

    if(maxLevel){
        return {
            x: 0
            , y: 0 
            , w: width 
            , h: width 
        };
    }

    return null;
}    

sigma.utils.computeHeight 
    = function( forest, options ) {
    var opt = options || {};

    forest.forEach( function( tree ) {
        _depthTravel( tree );
    } );

    function _depthTravel( node ) {
        var subTreeHeight = 0
            , children = node._wt_children
            ;

        children.forEach( function( child ) {
            subTreeHeight = Math.max( subTreeHeight, _depthTravel( child ) );
        } );

        node._wt_height = subTreeHeight + 1;
        if ( 'function' == typeof opt.onNode ) {
            opt.onNode( node );
        }

        return node._wt_height;
    }

};  
sigma.utils.computeHierarchyTreeRect
    = function(tree, offsetX){

    var maxLevel = tree._wt_maxlevel
        , leaves = tree._wt_leaves
        , height = maxLevel || 1
        , width = leaves || 1
        ;

    if(maxLevel){
        return {
            x: offsetX || 0
            , y: 0 
            , w: width 
            , h: height 
        };
    }

    return null;
}  

sigma.utils.computeLeaves
    = function(forest){

    forest.forEach(function(tree){

        // if there is a circuit
        if(tree._circuit){
            tree._circuit.forEach(function(node){
                depthTravel(node);
            });
        }
        else {
            depthTravel(tree);

        }

        function depthTravel(node){
            var children = node._wt_children
                , _leaves
                , leaves = 0
                ;
            if(children.length == 0){
                _leaves = [1];
            }
            else {
                _leaves = children.map(function(node){
                    return depthTravel(node);
                }); 
            }

            _leaves.forEach(function(item){
                leaves += item;
            });

            return ( node._wt_leaves = leaves );
        }

    });

}   

sigma.utils.computeLocalAndExternalNodes
    = function( subGraph, root, options ) {
    var opt = options || {}
        , subGraph = subGraph || { nodes: [], edges: [] }
        , nodes = subGraph.nodes
        , edges = subGraph.edges
        , nodesHash = {}
        , ancestorNodes = {}
        // local and external nodes
        , leNodes 
        , getAdjacentNodes = sigma.utils.getAdjacentNodes
        , nodeIdsOfSameLevel = {}
        ;

    if ( !root ) {
        return;
    }

    nodes.forEach( function( node ) { 
        nodesHash[ node.id ] = node;
    } );

    _sameLevelNodes( root );
    leNodes = _leNodes( root );
    _brothers ( root );
    return;

    function _leNodes( node ) {
        var localNodes = {}
            , externalNodes = {}
            , tmpNodes
            , adjacentNodes = {}
            , children = node._wt_children
            , id = node.id
            , _le_nodes
            ;

        if ( children ) {
            ancestorNodes[ id ] = 1;
            children.forEach( function( child ) {
                var le = _leNodes( child );

                _extend( localNodes, le.local );
                _extend( externalNodes, le.external );
            } );
            delete ancestorNodes[ id ];
        }

        localNodes[ id ] = 1;
        tmpNodes = getAdjacentNodes( node, nodes, edges );
        if ( tmpNodes ) {
            for ( var i = 0; i < tmpNodes.length; i++ ) {
                adjacentNodes[ tmpNodes[ i ].id ] = 1;
            }
        }
        _extend( externalNodes, adjacentNodes );
        for ( var i in ancestorNodes ) {
            delete externalNodes[ i ];
        }
        for ( var i in localNodes ) {
            delete externalNodes[ i ];
        }

        _le_nodes = {
            local: localNodes
            , external: externalNodes
            , adj: adjacentNodes
        };
        node._wt_lenodes = _le_nodes;
        return _le_nodes;
    }

    function _extend( dest, src ) {
        for( var i in src ) {
            dest[ i ] = src[ i ];
        }
    }

    function _sameLevelNodes( node ) {
        var children = node._wt_children
            , idArr = nodeIdsOfSameLevel[ node._wt_level ]
            ;

        if ( !idArr ) {
            idArr = nodeIdsOfSameLevel[ node._wt_level ]  = [];
        }
        idArr.push( node.id );

        if ( children ) {
            children.forEach( function( child ) {
                _sameLevelNodes( child ); 
            } );
        }
    }

    function _brothers ( node, siblingIds ) {
        var level = node._wt_level
            , children = node._wt_children
            , sameLevelIds = nodeIdsOfSameLevel[ level ]
            , id = node.id
            , leNodes = node._wt_lenodes
            , siblingIds = siblingIds || []
            , childIds

            // hash objects
            , localNodes = leNodes.local
            , externalNodes = leNodes.external
            , adjacentNodes = leNodes.adj

            , _di_brothers = {}
            , _indi_brothers = {}
            , _di_ex_brothers = {}
            , _indi_ex_brothers = {}
            , _other_ex_nodes = {}
            , _tmp
            ;

        if ( children ) {
            childIds = children.map( function( child ) { return child.id; } );
            children.forEach( function ( child ) {
                _brothers( child, childIds );
            } );
        }

        var flag
            , _id
            , _node
            , _localNodes
            , _tmpExternalNodes = {}
            ;

        _extend( _tmpExternalNodes, externalNodes );
        for( var j = siblingIds.length - 1; j >= 0; j-- ) {
            _id = siblingIds[ j ];
            if ( _id == id ) {
                continue;
            }

            if ( adjacentNodes[ _id ] ) {
                _di_brothers[ _id ] = 1;
                delete _tmpExternalNodes[ _id ];
            }
        } 

        for( j = sameLevelIds.length - 1; j >= 0; j-- ) {
            _id = sameLevelIds[ j ];
            if ( siblingIds.indexOf( _id ) >= 0 ) {
                continue;
            }

            if ( adjacentNodes[ _id ] ) {
                _di_ex_brothers[ _id ] = 1;
                delete _tmpExternalNodes[ _id ];
            }
            else if ( _tmpExternalNodes[ _id ] ) {
                _indi_ex_brothers[ _id ] = 1;
                delete _tmpExternalNodes[ _id ];
            }
        }

        for ( var i in _tmpExternalNodes ) {

            flag = 0;

            for( var j = siblingIds.length - 1; j >= 0; j-- ) {
                _id = siblingIds[ j ];

                // siblingIds contain node itself
                if ( _id == id ) {
                    continue;
                }

                _node = nodesHash[ _id ];
                _localNodes = _node._wt_lenodes.local;

                // B's localNodes contain `B`, eg. `AabB`, `AaB`
                if ( _localNodes[ i ] ) {
                    _indi_brothers[ _id ] = 1;
                    flag = 1;
                    delete _tmpExternalNodes[ i ];
                }
            } 

            // one external node produce one brother, that's enough.
            if ( flag ) {
                continue;
            }

            for( j = sameLevelIds.length - 1; j >= 0; j-- ) {
                _id = sameLevelIds[ j ];

                if ( siblingIds.indexOf( _id ) >= 0 ) {
                    continue;
                }

                _node = nodesHash[ _id ];
                _localNodes = _node._wt_lenodes.local;

                if ( _localNodes[ i ] ) {
                    _indi_ex_brothers[ _id ] = 1;
                    delete _tmpExternalNodes[ i ];
                }
            }

        }

        _extend( _other_ex_nodes, _tmpExternalNodes );

        _extend(
            node._wt_lenodes
            , {
                di_brothers: _transformToNodes( _di_brothers ) 
                , indi_brothers: _transformToNodes( _indi_brothers )
                , di_ex_brothers: _transformToNodes( _di_ex_brothers )
                , indi_ex_brothers: _transformToNodes( _indi_ex_brothers )
                , other_ex_nodes: _transformToNodes( _other_ex_nodes )
            }
        )

    }

    function _transformToNodes( obj ) {
        var nodes = [], node;
        for ( var i in obj ) {
            if ( node = nodesHash[ i ] ) {
                nodes.push( node );
            }
        }
        if( opt.sortBySubTreeSize ) {
            nodes.sort( function( a, b ) {
                // siblings those have a shorter and smaller subtree
                // are close to each other
                return ( 
                    a._wt_height + a._wt_leaves
                        - b._wt_height - b._wt_leaves
                );
            } );
        }
        return nodes;
    }

};   
sigma.utils.computeNetworkInfo
    = function( nodes, edges, options ) {

    var nodesIndex = {}
        , edgesIndex = {}
        , inNeighborsIndex = {}
        , outNeighborsIndex = {}
        , allNeighborsIndex = {}
        , inNeighborsCount = {}
        , outNeighborsCount = {}
        , allNeighborsCount = {}
        ;

    nodes.forEach( function( node ) {
        var id = node.id;
        nodesIndex[ id ] = node;

        inNeighborsIndex[ id ] = {}; 
        outNeighborsIndex[ id ] = {}; 
        allNeighborsIndex[ id ] = {}; 
        inNeighborsCount[ id ] = 0; 
        outNeighborsCount[ id ] = 0; 
        allNeighborsCount[ id ] = 0; 
    } );

    edges.forEach( function( edge ) {
        var id = edge.id;
        edgesIndex[ id ] = edge;

        inNeighborsIndex[ edge.target ][ edge.source ] =
            inNeighborsIndex[ edge.target ][ edge.source ] || {};
        inNeighborsIndex[ edge.target ][ edge.source ][ id ] = edge;

        outNeighborsIndex[ edge.source ][ edge.target ] =
            outNeighborsIndex[ edge.source ][ edge.target ] || {};
        outNeighborsIndex[ edge.source ][ edge.target ][ id ] = edge;

        allNeighborsIndex[ edge.target ][ edge.source ] =
            allNeighborsIndex[ edge.target ][ edge.source ] || {};
        allNeighborsIndex[ edge.target ][ edge.source ][ id ] = edge;

        allNeighborsIndex[ edge.source ][ edge.target ] =
            allNeighborsIndex[ edge.source ][ edge.target ] || {};
        allNeighborsIndex[ edge.source ][ edge.target ][ id ] = edge;

        inNeighborsCount[ edge.target ]++;
        outNeighborsCount[ edge.source ]++;
        allNeighborsCount[ edge.target ]++;
        allNeighborsCount[ edge.source ]++;
    } );

    return {
        nodes: function( nodeId ) {
            if ( typeof nodeId !== undefined ) {
                return nodesIndex[ nodeId ];
            }
            else {
                return nodes;
            }
        }
        , edges: function( edgeId ) {
            if ( typeof edgeId !== undefined ) {
                return edgesIndex[ edgeId ];
            }
            else {
                return edges;
            }
        }
        , degree: function( nodeId ) {
            return allNeighborsCount[ nodeId ];
        }
        , allNeighborsIndex: function( nodeId ) {
            var indices = [];
            for ( var key in allNeighborsIndex[ nodeId ] ) {
                indices.push( key );
            }
            return indices;
        }
        , allNeighbors: function( nodeId ) {
            var me = this
                , indices = me.allNeighborsIndex( nodeId )
                , neighbors = []
                ;
            indices.forEach( function( index ) {
                neighbors.push( me.nodes( index ) );
            } );
            return neighbors;
        }
    };
};  
sigma.utils.computeNodesExtraInfo
    = function( nodes, edges, options ){

    if ( !nodes || !nodes.length || !edges || !edges.length ) {
        return;
    } 

    var nodeHashes = {};

    nodes.forEach( function( node ) {
        delete node._extra_info;
        nodeHashes[ node.id ] = node;
    } );

    edges.forEach( function( edge ) {
        var source = edge.source
            , target = edge.target
            , label = edge.label || ''
            , sourceNode = nodeHashes[ source ]
            , targetNode = nodeHashes[ target ]
            , _info, _labels
            ;

        // maybe nodes and edges are not matched
        if ( sourceNode) {
            _info = sourceNode._extra_info = sourceNode._extra_info || {};
            _labels = _info.edgeLabels = _info.edgeLabels || {}; 
            _labels[ target ] = label;
        }

        if ( targetNode) {
            _info = targetNode._extra_info = targetNode._extra_info || {};
            _labels = _info.edgeLabels = _info.edgeLabels || {}; 
            _labels[ source ] = label;
        }
    } );
}
sigma.utils.depthTravel
    = function(nodes, edges, root, callbacks) {

    if(!nodes || !nodes.length){
        return;
    }

    var cbs = callbacks || {}
        , visitedNodes = {}
        , node = root || nodes[0]
        , children
        ;

    edges = edges || [];

    node._dt_level = 1;
    _depthTravel(node);

    function _depthTravel(node){
        if(visitedNodes[node.id]){
            return;
        }
        visitedNodes[node.id] = 1;

        if(typeof cbs['onNode'] == 'function'){
            cbs['onNode'](node);
        }

        var children = getChildren(node); 
        if(children.length > 0){
            children.forEach(function(child){
                _depthTravel(child);
            });
        }
    }

    function getChildren(node){
        var id = node.id
            , children = []
            , childId
            , child
            ;
        edges.forEach(function(edge){
            if(
                edge.source == id
                || edge.target == id
              ){
                childId = edge.source;
                if(childId == id){
                    childId = edge.target;
                }
                if(!visitedNodes[childId]){
                    child = sigma.utils.getNodeById(nodes, childId);
                    child._dt_level = node._dt_level + 1;
                    children.push(child);
                }
            }
        });
        return children;
    }

};

sigma.utils.getAdjacentNodes
    = function( node, nodes, edges ) {
    var retNodes = [];

    if ( !node || !nodes || !edges ) {
        return retNodes;
    }

    edges.forEach(function(edge){
        var adjNodeId, adjNode;

        if(edge.source == node.id){
            adjNodeId = edge.target;
        }

        if(edge.target == node.id){
            adjNodeId = edge.source;
        }

        if(!adjNodeId){
            return;
        }

        if(adjNodeId != node.id){
            if(nodes.map(function(node){return node.id;})
                .indexOf(adjNodeId) >= 0){
                adjNode = sigma.utils.getNodeById(nodes, adjNodeId);
                retNodes.push(adjNode);
            }
        }
    });

    return retNodes;
};
sigma.utils.getAngleInput
    = function(fromNode, toNode, options){

    var opt = options || {} 
        , readPrefix = opt.readPrefix || ''
        , dy = fromNode[readPrefix + 'y'] - toNode[readPrefix + 'y']
        , dx = fromNode[readPrefix + 'x'] - toNode[readPrefix + 'x']
        , angleInput
        ;
    sin = dy / Math.sqrt( 
            Math.pow(dx, 2) + Math.pow(dy, 2) 
        );
    cos = dx / Math.sqrt(
            Math.pow(dx, 2) + Math.pow(dy, 2) 
        );

    if( sin >= 0 ) {
        angleInput = Math.acos(cos);
    }
    else {
        angleInput = 2 * Math.PI - Math.acos(cos);
    }

    return angleInput;
}; 
sigma.utils.getCircleForest
    = function(nodes, edges, options){

    var opt = options || {} 
        ;

    if ( opt.makeMaxDegreeNodeRoot ) {
        nodes = sigma.utils.sortByNodesDegree( nodes, edges, 1 );
    }

    var circuits
        , circuit
        , tree = opt.root || nodes[0]
        , excludes
        , nodesVisited = {}
        , forest = []
        ;

    edges = edges || [];

    do {
        if ( opt.useComplicatedLoop ) {
            // todo: performance
            circuits = sigma.utils.getComplicatedLoops( nodes, edges, { root: tree } )
                        .complicated;
        }
        else {
            circuits = sigma.utils.getCircuits(nodes, edges, tree);
        }

        excludes = {};

        if(circuits.length > 0){
            circuits.sort(function(a, b){
                return b.length - a.length;
            });

            // the longest circuit
            tree._circuit = circuit = circuits[0]; 

            circuit.forEach(function(_node){
                excludes[_node.id] = 1;
            });

            circuit.forEach(function(_node){
                sigma.utils.widthTravel(
                    nodes
                    , edges
                    , _node
                    , {
                        onNode: function(__node){
                            nodesVisited[__node.id] = 1;

                            if ( 'function' == typeof opt.sortChildren ) {
                                __node._wt_children.sort( opt.sortChildren );
                            }
                            else {
                                __node._wt_children.sort( function( a, b ) {
                                    var _ea = a._extra_info
                                        , _eb = b._extra_info
                                        , _eaL, _ebL
                                        ;

                                    if ( !_ea || !_eb ) {
                                        return 0;
                                    }

                                    _eaL = _ea.edgeLabels;
                                    _ebL = _eb.edgeLabels;
                                    if ( !_eaL || !_ebL ) {
                                        return 0;
                                    }

                                    _eaL = _eaL[ __node.id ];
                                    _ebL = _ebL[ __node.id ];
                                    if ( !_eaL || !_ebL ) {
                                        return 0;
                                    }

                                    return _eaL.localeCompare( _ebL );
                                } );
                            }

                        }
                    }
                    , excludes
                );
            });
        }
        else {
            sigma.utils.widthTravel(
                nodes
                , edges
                , tree
                , {
                    onNode: function(__node){
                        nodesVisited[__node.id] = 1;

                        if ( 'function' == typeof opt.sortChildren ) {
                            __node._wt_children.sort( opt.sortChildren );
                        }
                        else {
                            __node._wt_children.sort( function( a, b ) {
                                var _ea = a._extra_info
                                    , _eb = b._extra_info
                                    , _eaL, _ebL
                                    ;

                                if ( !_ea || !_eb ) {
                                    return 0;
                                }

                                _eaL = _ea.edgeLabels;
                                _ebL = _eb.edgeLabels;
                                if ( !_eaL || !_ebL ) {
                                    return 0;
                                }

                                _eaL = _eaL[ __node.id ];
                                _ebL = _ebL[ __node.id ];
                                if ( !_eaL || !_ebL ) {
                                    return 0;
                                }

                                return _eaL.localeCompare( _ebL );
                            } );
                        }

                    }
                }
            );
        }

        forest.push(tree);

    } while((tree = hasMore()));

    function hasMore(){
        for(var i=0; i<nodes.length; i++){
            if(!nodesVisited[nodes[i].id]){
                return nodes[i];
            }
        }
        return 0;
    } 

    return forest;

};
sigma.utils.getCircuits
    = function(nodes, edges, root) {

    var nLen = nodes.length || 1
        , eLen = edges.length || 1
        ;

    if(eLen <= 50 && eLen / nLen <= 1.25){
        return sigma.utils.getCircuitsSlow(nodes, edges, root); 
    }
    else { 
        return sigma.utils.getCircuitsFast(nodes, edges, root); 
    }

};

sigma.utils.getCircuitsFast
    = function(nodes, edges, root) {

    if(!nodes || !nodes.length){
        return;
    }

    var me = this
        , root = root || nodes[0]
        , visitedNodes = {}
        , children
        , path = []
        , circuits = []
        ;

    edges = edges || [];

    _depthTravel(root);
    return circuits;

    function _depthTravel(node){
        if(visitedNodes[node.id]){
            return;
        }
        visitedNodes[node.id] = 1;
        path.push(node);

        var children = getChildren(node); 
        if(children.length > 0){
            children.forEach(function(child){
                _depthTravel(child);
            });
        }
        path.pop(node);
    }

    function getChildren(node){
        var id = node.id
            , children = []
            , childId
            , child
            ;
        edges.forEach(function(edge){
            if(
                edge.source == id
                || edge.target == id
              ){
                childId = edge.source;
                if(childId == id){
                    childId = edge.target;
                }

                if(childId == root.id && path.length > 2){
                    circuits.push(path.slice());  
                }                        

                if(!visitedNodes[childId]){
                    child = sigma.utils.getNodeById(nodes, childId);
                    children.push(child);
                }

            }
        });
        return children;
    }

};    

sigma.utils.getCircuitsSlow
    = function(nodes, edges, root) {

    if(!nodes || !nodes.length){
        return;
    }

    var me = this
        , root = root || nodes[0]
        , children
        , path = []
        , circuits = []
        , isStop = 0
        ;

    edges = edges || [];

    root._dt_level = 1;
    _depthTravel(root);
    return circuits;

    function _depthTravel(node){
        if(isStop){
            return;
        }

        if(!isNodeInPath(node)){
            path.push(node);

            var children = getChildren(node); 
            if(children.length > 0){
                children.forEach(function(child){
                    _depthTravel(child);
                });
            }

            path.pop(node);
        }
        else if(node.id == root.id
            && path.length > 2){
            circuits.push(path.slice());  
            if(circuits.length >= 20){
                isStop = 1;
            }
        }
    }

    function isNodeInPath(node){
        return path.map(function(_node){return _node.id;})
            .indexOf(node.id)
            != -1
            ;
    }

    function getChildren(node){
        var id = node.id
            , children = []
            , childId
            , child
            ;

        if(node._tmp_children){
            return node._tmp_children; 
        }

        edges.forEach(function(edge){
            if(
                edge.source == id
                || edge.target == id
              ){
                childId = edge.source;
                if(childId == id){
                    childId = edge.target;
                }

                child = sigma.utils.getNodeById(nodes, childId);
                children.push(child);
            }
        });

        return (node._tmp_children = children);
    }

};
sigma.utils.getClusterLevels
    = function(numOfFirstLevel, totalNum){

    var i = 1
        , step = numOfFirstLevel
        , all = 0
        ;

    do {
        all += i * step;
        i++;
    }
    while(all < totalNum);
    return i - 1; 
}   
sigma.utils.getDummyEdges 
    = function( fromNode, toNodes, options ) {
    var opt = options || {} 
        , edges = []
        , edge
        ;

    // assertions
    if ( !fromNode ) {
        throw new Error( 'sigma.utils.getDummyEdges: `fromNode` is undefined' );
    }
    if ( !toNodes ) {
        throw new Error( 'sigma.utils.getDummyEdges: `toNodes` is undefined' );
    }
    if ( !toNodes.length ) {
        throw new Error( 'sigma.utils.getDummyEdges: `toNodes.length` is zero' );
    }

    for ( var i = 0; i < toNodes.length; i++ ) {
        edge = {
            source: fromNode.id
            , target: toNodes[ i ].id
            , size: opt.size || 1
            , color: opt.color || '#0f0'
            , hover_color: opt.hover_color || '#f00'
            , _isdummy: 1
        };
        edge.id = 'dummyedge_' 
            + new Date().getTime()
            + '_' + ( Math.random() + '' ).substr( 2, 6 )
            ;
        edges.push( edge );
    }

    return edges;
};  
sigma.utils.getDummyNode = function( options ) {
    var opt = options || {} 
        , node = {
            x: +opt.x === opt.x ? opt.x : null
            , y: +opt.y === opt.y ? opt.y : null
            , size: opt.size || 1
            , color: opt.color || '#f00'
            , _isdummy: 1
        };

    node.id = 'dummynode_' 
        + new Date().getTime()
        + '_' + ( Math.random() + '' ).substr( 2, 6 )
        ;

    node.label = node.id;

    return node;
};   
sigma.utils.getGraphFromTree
        = function( tree, allEdges ) {
        var nodes = []
            , nodesHash = {}
            , edges = []
            ;

        allEdges = allEdges || [];
        depthTravel( tree );
        allEdges.forEach( function( edge ) {
            if ( nodesHash[ edge.source ]
                || nodesHash[ edge.target ] ) {
                edges.push( edge );
            }
        } );
        return {
            nodes: nodes
            , edges: edges
        };

        function depthTravel( node ) {
            nodes.push( node );
            nodesHash[ node.id ] = 1;
            if ( node._wt_children ) {
                node._wt_children.forEach( function( child ) {
                    depthTravel( child );
                });
            }
        }
    };
sigma.utils.getGridLayout
    = function(nodes, options){

    if(!nodes || !nodes.length){
        return;
    }

    var opt = options || {}
        , r = Math.ceil(Math.sqrt(nodes.length))
        , i = 0
        , j = 0
        , nodeSize = nodes[0].size || 1
        , step = opt.space || nodeSize  * 3 
        , center = opt.center
        , rect
        , ltPoint
        ;

    if(!center){
        rect = sigma.utils.getNodesRect(nodes, opt);
        center = {
            x: rect.x + rect.w / 2
            , y: rect.y + rect.h / 2
        };
    }

    ltPoint = {
        x: center.x - step * ( r - 1 ) / 2
        , y: center.y - step * ( r - 1 ) / 2
    };

    nodes.forEach(function(node){
        node.grid_x = j * step + ltPoint.x;
        node.grid_y = i * step + ltPoint.y; 
        j++;
        if(j >= r) {
            j = 0;
            i++;
        }
    });

    return nodes;
};
sigma.utils.getLayoutForest
    = function(nodes, edges, options){

    var opt = options || {}
        ;

    if ( opt.makeMaxDegreeNodeRoot ) {
        nodes = sigma.utils.sortByNodesDegree( nodes, edges, 1 );
    }

    var nodesVisited = {}
        , forest = []
        , node = opt.dummyRoot 
            || opt.root 
            || nodes[ 0 ] 
        , excludes = opt.excludes
        ;

    do {
        sigma.utils.widthTravel(
            nodes
            , edges
            , node
            , {
                onNode: function(__node){
                    nodesVisited[__node.id] = true;
                    if ( 'function' == typeof opt.sortChildren ) {
                        __node._wt_children.sort( opt.sortChildren );
                    }
                    else {
                        // take `dummyNode` into considerations
                        __node._wt_children.sort( function( a, b ) {
                            var _ea = a._extra_info
                                , _eb = b._extra_info
                                , _eaL, _ebL
                                ;

                            if ( !_ea || !_eb ) {
                                return 0;
                            }

                            _eaL = _ea.edgeLabels;
                            _ebL = _eb.edgeLabels;
                            if ( !_eaL || !_ebL ) {
                                return 0;
                            }

                            _eaL = _eaL[ __node.id ];
                            _ebL = _ebL[ __node.id ];
                            if ( !_eaL || !_ebL ) {
                                return 0;
                            }

                            return _eaL.localeCompare( _ebL );
                        } );
                    }

                    // do extra processing using `onNode` callback
                    if ( 'function' == typeof opt.onNode ) {
                        opt.onNode( __node );
                    }
                }
            } 
            , excludes
        )
        forest.push(node);
        ;
    } while((node = hasMore()));

    function hasMore(){
        for(var i=0; i<nodes.length; i++){
            if(!nodesVisited[nodes[i].id]){
                return nodes[i];
            }
        }
        return 0;
    }

    return forest;
};
sigma.utils.getMaxDegreeNode
    = function(nodes, edges){

    if(!nodes || !edges) {
        return null;
    }

    var degreeArr = [] 
        , degree
        , node, edge, i, j
        ;

    for(i=0; i<nodes.length; i++){
        node = nodes[i];
        degree = {
            id: node.id
            , data: 0
        };
        degreeArr.push(degree);

        for(j=0; j<edges.length; j++){
            edge = edges[j]; 
            if(
                edge.source  == node.id
                || edge.target  == node.id
            ){
                degree.data++;
            }
        }
    }
    degreeArr.sort(function(a, b){
        return b.data - a.data;
    });

    return degreeArr.length
        ? sigma.utils.getNodeById(nodes, degreeArr[0].id)
        : null;

};

sigma.utils.getNodeById
    = function(nodes, id){
        if(!nodes) {
            return null;
        }
        for(var i=0; i<nodes.length; i++){
            if(nodes[i].id == id){
                return nodes[i];
            }
        }
        return null;
    } 


sigma.utils.getNodesFromTree
    = function( tree ) {
    var nodes = [];

    depthTravel( tree );
    return nodes;

    function depthTravel( node ) {
        nodes.push( node );
        if ( node._wt_children ) {
            node._wt_children.forEach( function( child ) {
                depthTravel( child );
            } );
        }
    }
};  
sigma.utils.getNodesRect
    = function(nodes, options){

    var opt = options || {}
        , xMin = Infinity
        , yMin = Infinity
        , xMax = -Infinity
        , yMax = -Infinity
        , readPrefix = opt.readPrefix || ''
        , ignoreNodeSize = typeof opt.ignoreNodeSize == 'undefined'
            ? true : opt.ignoreNodeSize
        , node, i, x, y, size
        ;

    i = nodes.length - 1;
    while(i >= 0){
        node = nodes[i];
        x = node[readPrefix + 'x'] || 0;
        y = node[readPrefix + 'y'] || 0;
        size = ignoreNodeSize 
            ? 0 : node[readPrefix + 'size'] || node['size'] || 0.2;
        if(x - size < xMin){
            xMin = x - size;
        }
        if(x + size > xMax){
            xMax = x + size;
        }
        if(y - size < yMin){
            yMin = y - size;
        }
        if(y + size > yMax){
            yMax = y + size;
        }
        i--;
    }

    if(nodes.length == 0){
        xMin = 0;
        yMin = 0;
        xMax = 0;
        yMax = 0;
    }

    return {
        x: xMin
        , y: yMin
        , w: xMax - xMin
        , h: yMax - yMin
    };
}   

sigma.utils.getNodesWithinDistance
    = function( nodes, edges, from, distance, options ) {
    var opt = options || {}
        , nodesWithinDistance = []
        ;

    if ( !nodes || !edges || !from || distance < 0 ) {
        throw Error( 'sigma.utils.getRelationInfoAtDistance: wrong arguments' );
    }

    sigma.utils.widthTravel(
        nodes
        , edges
        , from
    );

    sigma.utils.depthTravel(
        nodes
        , edges
        , from
        , {
            onNode: function( node ) {
                if ( node._wt_level <= distance + 1 ) {
                    nodesWithinDistance.push( node );
                }
            }
        }
    );

    return nodesWithinDistance;
};
sigma.utils.getNumOfFirstClusterLevel
    = function(totalNum, max, min){

    var max = max || 18 
        , min = min || 1
        , i = 1
        , t = 0
        , m
        ;
    while(1){
        t += i; 
        m = Math.ceil(totalNum / t);
        if(m >= min && m <= max){
            return m;
        }
        i++;
    }
} 
sigma.utils.getRelationInfoAtDistance
    = function( nodes, edges, from, distance, options ) {
    var opt = options || {}
        , nodesAtDistance = []
        , pathGraph = { nodes: [], edges: [] }
        , nodesHash = {}
        , edgesHash = {}
        ;

    if ( !nodes || !edges || !from || distance < 0 ) {
        throw Error( 'sigma.utils.getRelationInfoAtDistance: wrong arguments' );
    }

    sigma.utils.widthTravel(
        nodes
        , edges
        , from
        , {
            onNode: function( node ) {
                node._wt_children.forEach( function( child ) {
                    child._wt_parent = node;
                } );
            }
        }
    );

    sigma.utils.depthTravel(
        nodes
        , edges
        , from
        , {
            onNode: function( node ) {
                if ( node._wt_level == distance + 1 ) {
                    nodesAtDistance.push( node );
                }
            }
        }
    );

    nodesAtDistance.forEach( function( node ) {
        var subPathGraph = _getPathGraphToNode( node );

        subPathGraph.nodes.forEach( function( _node ) {
            if ( !nodesHash[ _node.id ] ) {
                pathGraph.nodes.push( _node );
                nodesHash[ _node.id ] = 1;
            }
        } );
        subPathGraph.edges.forEach( function( _edge ) {
            if ( !edgesHash[ _edge.id ] ) {
                pathGraph.edges.push( _edge );
                edgesHash[ _edge.id ] = 1;
            }
        } );
    } );

    return {
        nodes: nodesAtDistance
       , pathGraph: pathGraph
    };

    function _getPathGraphToNode( to ) {
        var subGraph = { nodes: [], edges: [] }
            , prevNode
            , currentNode = sigma.utils.getNodeById( nodes, to.id )
            ;
        do {
            subGraph.nodes.push( currentNode );
            if ( prevNode ) {
                subGraph.edges.push( _getEdges( currentNode, prevNode ) );
            }
            prevNode = currentNode;
            currentNode = currentNode._wt_parent;
        } while ( currentNode );

        if ( prevNode.id != from.id ) {
            throw Error( 
                'getRelationInfoAtDistance:_getPathGraphToNode: something wrong occur' 
            );
        }
        subGraph.nodes = subGraph.nodes.reverse();
        subGraph.edges = subGraph.edges.reverse();
        return subGraph;
    }

    function _getEdges( sourceNode, targetNode ) {
        var i = 0, len = edges.length, edge;
        while( i < len ) {
            edge = edges[ i ];
            if ( edge.source == sourceNode.id
                    && edge.target == targetNode.id
                || edge.target == sourceNode.id
                    && edge.source == targetNode.id ) {
                return edge;
            }
            i++;
        }
        throw Error( 
            'getRelationInfoAtDistance:_getEdges: something wrong occur' 
        );
    }
};
sigma.utils.getShortestPath
    = function( nodes, edges, from, to, options ) {
    var opt = options || {}
        , forest
        , extend = sigma.utils.extend
        , prevNode
        , currentNode
        , subGraph = { nodes: [], edges: [] } 
        ;

    if ( !nodes || !edges || !from || !to ) {
        throw Error( 'sigma.utils.getShortestPath: wrong arguments' );
    }

    forest = sigma.utils.getLayoutForest( 
        nodes
        , edges
        , extend( opt, {
            root: from
            , onNode: function( node ) {
                node._wt_children.forEach( function( child ) {
                    child._wt_parent = node;
                } );
            } 
        } )
    );

    currentNode = sigma.utils.getNodeById( nodes, to.id );
    do {
        subGraph.nodes.push( currentNode );
        if ( prevNode ) {
            subGraph.edges.push( _getEdges( currentNode, prevNode ) );
        }
        prevNode = currentNode;
        currentNode = currentNode._wt_parent;
    } while ( currentNode );

    if ( prevNode.id != from.id ) {
        subGraph.nodes.length = 0;
        subGraph.edges.length = 0;
    }
    subGraph.nodes = subGraph.nodes.reverse();
    subGraph.edges = subGraph.edges.reverse();

    return subGraph;

    function _getEdges( sourceNode, targetNode ) {
        var i = 0, len = edges.length, edge;
        while( i < len ) {
            edge = edges[ i ];
            if ( edge.source == sourceNode.id
                    && edge.target == targetNode.id
                || edge.target == sourceNode.id
                    && edge.source == targetNode.id ) {
                return edge;
            }
            i++;
        }
        throw Error( 'getShortestPath:_getEdges: no matched edge found.' );
    }
};
sigma.utils.incLayoutGrid
    = function(nodes, newNodes, selectedNodes, options){

    if(!newNodes || !newNodes.length
        || !nodes || !nodes.length){
        return;
    }

    var opt = options || {} 
        , rect
        , center
        , newCenter
        , oldNodes
        , newWidth = ( Math.ceil(Math.sqrt(newNodes.length)) - 1 ) * ( opt.space || 50 ) 
            + ( newNodes[0].size || + 20 )
        , gnr = sigma.utils.getNodesRect
        ;

    selectedNodes = selectedNodes || [];
    if(!selectedNodes.length){
        selectedNodes = nodes;
    }
    rect = gnr(nodes, opt);
    center = {
        x: rect.x + rect.w / 2
        , y: rect.y + rect.h / 2
    };
    newCenter = {
        x: rect.x + rect.w + newWidth
        , y: rect.y + rect.h / 3
    };



    rect = gnr(selectedNodes, opt);
    // from selected nodes' center point 
    newNodes.forEach(function(node){
        node.x = rect.x + rect.w / 2; 
        node.y = rect.y + rect.h / 2;
    });

    // prepare for the next animation
    nodes.forEach(function(node){
        node.grid_x = node.x;
        node.grid_y = node.y;
    });

    opt.center = newCenter;
    sigma.utils.getGridLayout(newNodes, opt);

};



sigma.prototype.incLayoutGrid = function(
    newNodes, selectedNodes, options) {
    var me = this
        , nodes = me.graph.nodes()
        ;

    sigma.utils.incLayoutGrid(
        nodes
        , newNodes
        , selectedNodes
        , options
    );

    return me;
};
sigma.utils.interpolatesAngle
    = function(
        angleRange
        , numOfFirstLevel
        , levels
        , angleStart
    ) {
    var retArr = []
        , angleStart = angleStart || 0
        , numOfCurrentLevel
        , anglesOfCurrentLevel
        , stepOfCurrentLevel
        , angleOffset
        , i, j
        ;

    for(i=0; i<levels; i++){
        numOfCurrentLevel = ( i + 1 ) * numOfFirstLevel;
        anglesOfCurrentLevel = [];
        stepOfCurrentLevel = angleRange / numOfCurrentLevel;
        angleOffset = stepOfCurrentLevel * ( i == 0 ? 0 : 0.5 );
        angleOffset += angleStart;

        retArr.push(anglesOfCurrentLevel);
        for(j=0; j<numOfCurrentLevel; j++){
            anglesOfCurrentLevel.push(
                stepOfCurrentLevel * j + angleOffset
            );
        }
    }

    return retArr;
};
sigma.utils.interpolatesAngleNew
    = function(
        totalNum
        , radiusStep
        , nodeOccupiedSpace
        , options
    ) {

    var opt = options || {}
        , PI = Math.PI
        , angleInputs = opt.angleInputs || []
        , firstLevelMinAngleSpacing = opt.firstLevelMinAngleSpacing || PI / 180 * 5
        , angleAdjustmentRatio = opt.angleAdjustmentRatio || 0.3
        , angleSpacingRatio = opt.angleSpacingRatio || 0.5
        , angleWellDistributed = opt.angleWellDistributed || 0
        , radiusWellDistributed = opt.radiusWellDistributed || 0
        , retArr = []

        , level
        , radius
        , firstLevelAngleStep 
        , currentLevelAngleStep
        , minCurrentLevelAngleStep
        , realCurrentLevelAngleStep
        , currentLevelAngles
        , angleRanges = []
        , i, j, len, slotStart, slotEnd
        , isCircle = 0
        ;

    // assertions
    if ( totalNum !== +totalNum ) {
        throw new Error( 'sigma.utils.interpolatesAngleNew: totalNum not specified!' );
    }

    if ( radiusStep <= 0 ) {
        throw new Error( 'sigma.utils.interpolatesAngleNew: radiusStep error!' );
    }

    if ( nodeOccupiedSpace <= 0 ) {
        throw new Error( 'sigma.utils.interpolatesAngleNew: nodeOccupiedSpace error!' );
    }

    if ( totalNum <= 0 ) {
        return retArr;
    }

    angleInputs.forEach( function( angle ) {
        if ( isNaN( angle ) ) {
            throw new Error( 'sigma.utils.interpolatesAngleNew: invalid angleInputs!' );
        }
    } );

    // get angleRanges
    if ( angleInputs.length <= 0 ) {
        angleRanges.push( [ -0.5 * PI, 1.5 * PI ] );
        isCircle = 1;
    }
    else {
        angleInputs.sort( function( a, b ) { return a - b; } );
        len = angleInputs.length;
        for( i = 0; i < len; i++ ) {
            slotStart = angleInputs[ i ];
            j = i < len - 1 ? i + 1 : 0;  
            slotEnd = angleInputs[ j ];

            if ( j == 0 ) {
                slotEnd += 2 * PI;
            }

            if ( slotEnd - slotStart > 2 * firstLevelMinAngleSpacing ) {
                angleRanges.push( [ slotStart, slotEnd ] );
            }
        } 

        // the bigger space first
        angleRanges.sort( function( a, b ) {
            return ( b[ 1 ] - b[ 0 ] ) - ( a[ 1 ] - a[ 0 ] );
        } );
    }

    // if empty angleRanges, return
    if ( !angleRanges.length ) {
        return [];
    }

    firstLevelAngleStep = nodeOccupiedSpace / ( 2 * radiusStep );
    realCurrentLevelAngleStep = currentLevelAngleStep = firstLevelAngleStep;
    currentLevelAngles = [];

    if ( isCircle ) {
        // circle layout
        angleSpacingRatio = 0;
    }

    i = 0; // trace how many nodes
    level = 1;

    while ( 1 ) {

        angleRanges.forEach( function( range ) {

            var start = range[ 0 ]
                , end = range[ 1 ]
                , mid = ( start + end ) / 2
                , k = 1
                , angleOffset
                , angleAdjustment = currentLevelAngleStep 
                    * ( level % 2 == 0 ? 1 : -1 ) 
                    * ( level == 2 ? 0.5 : angleAdjustmentRatio )
                    * ( level == 1 ? 0 : 1 )
                , midAdjusted
                , breakFlag
                ;

            if ( angleWellDistributed ) {
                angleAdjustment = currentLevelAngleStep / 2 * ( level == 1 ? 0 : 1 );
            }
            midAdjusted = mid + angleAdjustment;

            currentLevelAngles.push( midAdjusted );
            i++;

            while( 1 ) {

                angleOffset = realCurrentLevelAngleStep * k;
                breakFlag = 1;
                if ( midAdjusted + angleOffset 
                    <= end - realCurrentLevelAngleStep * angleSpacingRatio ) {
                    currentLevelAngles.push( midAdjusted + angleOffset );
                    i++;
                    breakFlag = 0;
                } 

                // if circular, we need to prevent node overlapping.
                if ( isCircle ) {
                    if ( midAdjusted - angleOffset 
                        > start + realCurrentLevelAngleStep * angleSpacingRatio ) {
                        currentLevelAngles.push( midAdjusted - angleOffset );
                        i++;
                        breakFlag = 0;
                    } 
                }
                else {
                    if ( midAdjusted - angleOffset 
                        >= start + realCurrentLevelAngleStep * angleSpacingRatio ) {
                        currentLevelAngles.push( midAdjusted - angleOffset );
                        i++;
                        breakFlag = 0;
                    } 
                }

                if ( !breakFlag ) {
                    k++;
                }
                else {
                    break;
                }

            } 

        } );

        retArr.push( currentLevelAngles );

        if ( i > totalNum ) {
            break;
        }

        level++;
        if ( level > 2 ) {
            if ( radiusWellDistributed ) {
                // 2r, 3r, 4r, 5r, ...
                radius = ( level + 1 ) * radiusStep;
            }
            else {
                // 2r, 3r, 5r, 7r, ...
                radius = ( 2 * level - 1 ) * radiusStep;
            }

            if ( angleWellDistributed ) {
                minCurrentLevelAngleStep = nodeOccupiedSpace / radius;
                currentLevelAngleStep /= 2;
                realCurrentLevelAngleStep = currentLevelAngleStep;
                while( realCurrentLevelAngleStep < minCurrentLevelAngleStep ) {
                    realCurrentLevelAngleStep += currentLevelAngleStep;
                }
            }
            else {
                currentLevelAngleStep = nodeOccupiedSpace / radius;
                realCurrentLevelAngleStep = currentLevelAngleStep;
            }
        }
        currentLevelAngles = [];

    }

    return retArr;

};   
sigma.utils.layoutTreesByGrid
    = function(forest, options){

    var opt = options || {}
        , spaceGrid = opt.spaceGrid || {xSize: 40, ySize: 40}
        , grid = new Grid(spaceGrid.xSize, spaceGrid.ySize)
        , unit = opt.optimalDistance || 100 
        , prefix = opt.readPrefix || opt.writePrefix || ''
        , debug = opt.debug || 0
        ;


    forest.forEach(function(tree){
        _computeTreeRect(tree);
    });

    // sort by tree's area in decreasing order
    forest.sort(function(a, b){
        return b._tmp_rect.w * b._tmp_rect.h
            - a._tmp_rect.w * a._tmp_rect.h
            ;
    });

    forest.forEach(function(tree){
        var spaceBlock = _normalizeTreeRect(tree, unit) 
            ;

        grid.placeBlock(tree.id, spaceBlock, debug);
    });

    forest.forEach(function(tree){
        var spaceBlock = grid.getBlockRect(tree.id) 
            , hasCircuit = tree._circuit ? 1 : 0
            , dx = spaceBlock.gridPos.x * unit - spaceBlock.x
            , dy = spaceBlock.gridPos.y * unit - spaceBlock.y
            ;

        debug && console.log(tree.id, spaceBlock);

        // clear temporary attribures
        if(tree._tmp_rect){
            delete tree._tmp_rect;
        }

        // if there is a circuit
        if(tree._circuit){
            tree._circuit.forEach(function(node){
                __depthTravel(node);
            });
            delete tree._circuit;
        }
        else {
            __depthTravel(tree);
        }

        function __depthTravel(node){
            var children = node._wt_children
                ;

            node[prefix + 'x'] += dx;
            node[prefix + 'y'] += dy;

            if(children.length > 0){
                children.forEach(function(child){
                    __depthTravel(child);
                }); 
                // delete node._wt_children;
            }
        }
    });


    function _computeTreeRect(tree){
        var nodes = []
            , circuit = tree._circuit
            , rect
            ; 

        if(circuit){
            circuit.forEach(function(node){
                __depthTravel(node);
            });
        }
        else {
            __depthTravel(tree);
        }

        rect = sigma.utils.getNodesRect(
            nodes
            , {
                readPrefix: prefix
                , ignoreNodeSize: 0
            }
        );
        tree._tmp_rect = rect; 
        tree._node_count = nodes.length;
        nodes.length = 0;
        return rect;

        function __depthTravel(node){
            var children = node._wt_children
                , len = children.length
                ;

            if ( !node._isdummy ) {
                nodes.push(node);
            }
            if(len > 0){
                children.forEach(function(child){
                    __depthTravel(child); 
                });
            }
        }
    }

    function _normalizeTreeRect(tree, unit){
        var rect = tree._tmp_rect
            , spaceBlock = {}
            , extendRatio = 1
            ;

        spaceBlock.x = rect.x;
        spaceBlock.y = rect.y;
        if(tree._node_count > 1){
            // extendRatio = 1.2;
            extendRatio = 1;
        }

        // `extendRatio` is for reserved space for node-collision on boundaris
        spaceBlock.w = Math.ceil(rect.w * extendRatio / unit);
        spaceBlock.h = Math.ceil(rect.h * extendRatio / unit);

        if(spaceBlock.w * unit - rect.w < unit
            && tree._node_count > 1){
            spaceBlock.w++;
        }
        if(spaceBlock.h * unit - rect.h < unit
            && tree._node_count > 1){
            spaceBlock.h++;
        }

        return spaceBlock;
    }


};     
( function() {



function getDistance(node1, node2, options){
    var opt = options || {}
        , prefix = opt.readPrefix || ''
        , tip;

    if(!node1
        || !node2
        || isNaN(node1[prefix + 'x']) 
        || isNaN(node1[prefix + 'y'])
        || isNaN(node2[prefix + 'x'])
        || isNaN(node2[prefix + 'y'])
        ){
        tip = 'getDistance: nodes not exist or have no coordinates';
        if('function' == typeof opt.debugShow){
            opt.debugShow(tip, node1, node2);
        }
        else{
            throw new Error(tip);
        }
    }

    return Math.sqrt(
        Math.pow(node1[prefix + 'x'] - node2[prefix + 'x'], 2)
        + Math.pow(node1[prefix + 'y'] - node2[prefix + 'y'], 2)
    );
}  


function computeElectricalForce(
    node1, node2, distance, options){

    var opt = options || {}
        , relativeStrength = opt.relativeStrength
        , optimalDistance = opt.optimalDistance 
        , prefix = opt.readPrefix || ''
        , force = {
            dx: node2[prefix + 'x'] - node1[prefix + 'x']
            , dy: node2[prefix + 'y'] - node1[prefix + 'y']
        }
        , scale
        , zeroForce = {dx:0, dy:0}
        , tip
        ;

    if(optimalDistance !== +optimalDistance
        || relativeStrength !== +relativeStrength){
        tip = 'computeElectricalForce: optimalDistance or relativeStrength error';
        if('function' == typeof opt.debugShow){
            opt.debugShow(tip);
        }
        else{
            throw new Error(tip);
        }
        return zeroForce; 
    }

    scale = -relativeStrength * optimalDistance * optimalDistance
        / distance / distance;

    if(isNaN(scale) || Infinity == scale){
        tip = 'computeElectricalForce: NaN or Infinity scale';
        if('function' == typeof opt.debugShow){
            opt.debugShow(tip, 'scale: ' + scale);
        }
        else{
            console.log(tip, 'scale: ' + scale);
        }
        scale = -1;
    }

    force.dx *= scale;
    force.dy *= scale;

    return force;
} 


function computeRepulsionForce(
    node, quadTree, options) {

    var opt = options
        , zeroForce = {dx:0, dy:0, num:0}
        , distance
        , tree = quadTree
        , force
        , tip
        ;

    if(tree.mass <= 0){
        return zeroForce;
    }

    distance = getDistance(node, tree, opt);

    if(!opt || isNaN(opt.barnesHutTheta)){
        tip = 'computeRepulsionForce: options error!';
        if('function' == typeof opt.debugShow){
            opt.debugShow(tip, opt);
        }
        else {
            throw new Error(tip);
        }
    }

    if(tree.isLeaf || tree.mass == 1){
        if(distance < 1e-8){
            force = zeroForce;
        }
        else {
            force = computeElectricalForce(node, tree, distance, opt);  
        }
        force.num = 1;
    }
    else if(distance * opt.barnesHutTheta > tree.size){
        force = computeElectricalForce(node, tree, distance, opt); 
        force.dx *= tree.mass;
        force.dy *= tree.mass;
        force.num = 1;
    }
    else {
        force = zeroForce;
        force.num = 0;
        tree.children.forEach(function(child){
            var f = computeRepulsionForce(node, child, options);
            force.dx += f.dx;
            force.dy += f.dy;
            force.num += f.num;
        });
    }

    return force;
}  


function computeAttractionForce(
    node1, node2, options){

    var opt = options || {}
        , optimalDistance = opt.optimalDistance
        , prefix = opt.readPrefix || ''
        , force = {
            dx: node2[prefix + 'x'] - node1[prefix + 'x']
            , dy: node2[prefix + 'y'] - node1[prefix + 'y']
        }
        , distance = getDistance(node1, node2, opt)
        ;

    if(opt.optimalDistance == undefined){
        throw new Error('computeAttractionForce: optimalDistance not specified!');
    }

    force.dx *= distance / optimalDistance; 
    force.dy *= distance / optimalDistance;
    return force;
}   


sigma.utils.layoutYifanHu
    = function(nodes, edges, options){

    var opt = options || {}
        , nodes = nodes || [] 
        , edges = edges || [] 
        , prefix

        , quadTree
        , energyPrev = 0
        , energy = 0
        , step
        , isConverged = 0
        , progress = 0
        , iterations
        , energyChangeRatio

        , forest
        , layoutBalanced = opt.layoutBalanced || 0
        , spaceGrid = opt.spaceGrid || {
            xSize: 50
            , ySize: 50
        }
        ;

    // options
    opt.stepRatio = opt.stepRatio || 0.95;
    opt.relativeStrength = opt.relativeStrength || 0.2;
    opt.optimalDistance = opt.optimalDistance 
        || getOptimalDistance();
    opt.layoutBalancedSpace = opt.layoutBalancedSpace
        || opt.optimalDistance;
    opt.initialStep = opt.initialStep 
        || opt.optimalDistance / 5; 
    opt.quadTreeMaxLevel = opt.quadTreeMaxLevel || 8;
    opt.barnesHutTheta = opt.barnesHutTheta || 1.2;
    opt.convergenceThreshold = opt.convergenceThreshold || 1e-4;
    opt.maxIterations = opt.maxIterations || 2;

    prefix = opt.readPrefix || opt.writePrefix || '';
    step = opt.initialStep;
    iterations = opt.maxIterations; 

    debugShow('options', opt, '\n');

    // prepare prefix access
    nodes.forEach(function(node){
        node[prefix + 'x'] = node.x;
        node[prefix + 'y'] = node.y;
    });

    if(nodes.length <= 1){
        return {
            isConverged: 1
            , iterations: 0
        };
    }

    do {

        quadTree = buildBHQuadTree({nodes: nodes}
            , opt.quadTreeMaxLevel
            , {readPrefix: prefix}
        );

        nodes.forEach(function(node){
            node.dx = node.dy = 0;
        });

        nodes.forEach(function(node){
            var f = computeRepulsionForce(node, quadTree, opt);
            node.dx += f.dx;
            node.dy += f.dy;
        });

        // debugShow(1, nodes.map(function(node){
        //     return node.dx + ',' + node.dy
        // }), '\n');

        edges.forEach(function(edge){
            var n1 = sigma.utils.getNodeById(nodes, edge.source)
                , n2 = sigma.utils.getNodeById(nodes, edge.target)
                , f
                ;

            if(n1.id != n2.id){
                f = computeAttractionForce(n1, n2, opt);
                // debugShow('1.x', force);
                n1.dx += f.dx;
                n1.dy += f.dy;
                n2.dx -= f.dx;
                n2.dy -= f.dy;
            }
        });

        // debugShow(2, nodes.map(function(node){
        //     return node.dx + ',' + node.dy
        // }), '\n');

        // compute energy and move nodes according to force
        energyPrev = energy;
        energy = 0;
        nodes.forEach(function(node){
            var e, scale;

            e = node.dx * node.dx + node.dy * node.dy;
            scale = Math.sqrt(e);

            energy += e;

            if(!node.fixed){
                // normalized vector
                node.dx /= scale; 
                node.dy /= scale;

                node[prefix + 'x'] += step * node.dx;
                node[prefix + 'y'] += step * node.dy;
            }

            delete node.dx;
            delete node.dy;
        });

        // debugShow(3, nodes.map(function(node){
        //     return '[' + node.x 
        //         + ',' + node.y
        //         + '] => [' + node[prefix + 'x'] 
        //         + ',' + node[prefix + 'y'] 
        //         + ']'
        //         ;
        // }), '\n');

        // update step -- adaptive cooling
        updateStep();

        // check if converged
        energyChangeRatio = Math.abs(energy - energyPrev) / energy; 
        debugShow('energyChangeRatio', energyChangeRatio, energy, energyPrev);
        if(energyChangeRatio < opt.convergenceThreshold){
            isConverged = 1;
            debugShow('\n' + 4, 'converged!');
        }

    } while (!isConverged && --iterations);

    if(layoutBalanced){
        forest = sigma.utils.getLayoutForest(
            nodes
            , edges
        );
        sigma.utils.layoutTreesByGrid(
            forest
            , {
                optimalDistance: opt.layoutBalancedSpace
                , readPrefix: 'yfh_'
                , spaceGrid: spaceGrid 
            }
        );
    }

    return {
        isConverged: isConverged
        , iterations: opt.maxIterations - iterations
    };

    // optimalDistance = C^(1/3) * averageEdgeLength
    function getOptimalDistance(){
        var edgeLength = 0
            , len = edges.length
            , avgLength
            ;

        edges.forEach(function(edge){
            edgeLength += getDistance(
                sigma.utils.getNodeById(nodes, edge.source)
                , sigma.utils.getNodeById(nodes, edge.target)
                , {
                    readPrefix: prefix
                }
            );
        });
        avgLength = edgeLength / len;
        return Math.pow(opt.relativeStrength, 1/3) * avgLength;
    }

    function updateStep(){
        if( energy < energyPrev){
            progress++;
            if(progress >= 5){
                progress = 0;
                step /= opt.stepRatio;
            }
        }
        else{
            progress = 0;
            step *= opt.stepRatio;
        }
        debugShow('step', step);
    }

    function debugShow(){
        if('function' == typeof opt.debugShow){
            opt.debugShow.apply(window, arguments);
        }
    }

}; 





} )();
sigma.utils.layoutYifanHuV2
    = function( nodes, edges, options ) {

    var opt = options || {}
        , nodes = nodes || []
        , edges = edges || []
        , forest
        , layoutBalanced = opt.layoutBalanced || 0
        , spaceGrid = opt.spaceGrid || {
            xSize: 50
            , ySize: 50
        }
        , results = []
        , newOpt = Object.assign( {}, opt, { layoutBalanced: 0 } )
        ;

    console.log( newOpt );

    forest = sigma.utils.getLayoutForest( nodes, edges );

    forest.forEach( function( tree ) {

        var subGraph = sigma.utils.getGraphFromTree( tree, edges )
            , result
            ;

        result = sigma.utils.layoutYifanHu( 
            subGraph.nodes
            , subGraph.edges
            , newOpt
        );

        results.push( result );

    } );

    if ( layoutBalanced ) {
        sigma.utils.layoutTreesByGrid(
            forest
            , {
                optimalDistance: opt.layoutBalancedSpace
                , readPrefix: 'yfh_'
                , spaceGrid: spaceGrid
            }
        );
    }

    return results;

};   
sigma.utils.normalizeSophonNodes
    = function(nodes, options){

    if(!nodes){
        return null;
    }

    var opt = options || {} 
        , center = opt.center || {x:0, y:0}
        // , size = opt.size || 10
        , prefix = opt.readPrefix || ''
        , rect = sigma.utils.getNodesRect(nodes, opt) 
        , nodesCenter = {
            x: rect.x + rect.w / 2
            , y: rect.y + rect.h / 2
        }
        , dx = center.x - nodesCenter.x
        , dy = center.y - nodesCenter.y
        ;

    nodes.forEach(function(node){
        node[prefix + 'x'] += dx;
        node[prefix + 'y'] += dy;
        // node.size = size;
    });

    return nodes;
}
sigma.utils.sortByNodesDegree
    = function( nodes, edges, reverse ) {

    if(!nodes || !edges) {
        throw new Error( 'sigma.utils.sortByNodesDegree: empty `nodes` or `edges`' );
    }

    var degreeArr = [] 
        , retNodes = []
        , degree
        , node, edge, i, j
        ;

    for(i=0; i<nodes.length; i++){
        node = nodes[i];
        degree = {
            node: node
            , data: 0
        };
        degreeArr.push(degree);

        for(j=0; j<edges.length; j++){
            edge = edges[j]; 
            if(
                edge.source  == node.id
                || edge.target  == node.id
            ){
                degree.data++;
            }
        }
    }

    degreeArr.sort(function(a, b){
        return ( 
            reverse
                ? b.data - a.data
                : a.data - b.data
        );
    });

    degreeArr.forEach( function( item ) {
        retNodes.push( item.node );
    } );

    return retNodes;

};   
sigma.utils.widthTravel
    = function(nodes, edges, root, callbacks, excludes) {

    if(!nodes || !nodes.length){
        return;
    }

    var cbs = callbacks || {}
        , edges = edges || []
        , queue = []
        , parentAndSiblingNodes = excludes || {}
        , root = root || nodes[0]
        , node = root
        , children
        , maxLevel = 0
        ;

    node._wt_level = 1;
    maxLevel = Math.max( node._wt_level, maxLevel );
    queue.push(node);
    parentAndSiblingNodes[node.id] = 1;

    while((node = queue.splice(0, 1)).length > 0){
        node = node[0];
        children = getChildren(node); 
        node._wt_children = children;
        queue = queue.concat(children);

        if(typeof cbs['onNode'] == 'function'){
            cbs['onNode'](node);
        }
    }
    root._wt_maxlevel = maxLevel;

    function getChildren(node){
        var id = node.id
            , children = []
            , childId
            , child
            ;
        edges.forEach(function(edge){
            if(
                edge.source == id
                || edge.target == id
              ){
                childId = edge.source;
                if(childId == id){
                    childId = edge.target;
                }
                if(!parentAndSiblingNodes[childId]){
                    parentAndSiblingNodes[childId] = 1;
                    child = sigma.utils.getNodeById(nodes, childId);
                    child._wt_level = node._wt_level + 1;
                    maxLevel = Math.max( child._wt_level, maxLevel );
                    children.push(child);
                }
            }
        });
        return children;
    }

};
