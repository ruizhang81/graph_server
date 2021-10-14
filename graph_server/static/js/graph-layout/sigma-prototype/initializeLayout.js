sigma.prototype.initializeLayout
    = function( options ) {
    var me = this
        , opt = options || {}
        , tmpFields = [
            '_wt_children'
            , '_wt_leaves'
            , '_wt_maxlevel'
            , '_wt_height'
            , '_wt_level'
            , '_wt_dy'

            , 'hier_x'
            , 'hier_y'
            , 'yfh_x'
            , 'yfh_y'
            , 'circle_x'
            , 'circle_y'
            , 'grid_x'
            , 'grid_y'
            , 'resize_x'
            , 'resize_y'

            , 'fixed'
            , '_tmp_children'
            , '_loops'
            , '_circuit'
            , '_extra_info'
        ]
        ;

    me.graph.nodes().forEach( function( node ) {
        tmpFields.forEach ( function( field ) {
            delete node[ field ];
        } );
    } );

    if ( !opt.clearOnly ) {
        me.graph.computeNodesExtraInfo( options );
    }

    return me;
}; 
