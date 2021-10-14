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
