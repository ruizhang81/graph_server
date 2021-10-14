sigma.prototype.clusterLeafNodes
    = function( options ) {

    var me = this
        , opt = options || {}
        , g = me.graph
        , subGraph = me.graph.getSubGraph( opt )
        , nodes = subGraph.nodes
        , edges = subGraph.edges
        , networkInfo = sigma.utils.computeNetworkInfo( nodes, edges )
        , leafNodes = []
        , parentNodesHash = {}
        , sortLeaves = opt.sortLeaves || null
        , layoutStarThreshold = opt.layoutStarThreshold || 0
        ;

    nodes.forEach( function( node ) {
        if ( networkInfo.degree( node.id ) == 1 ) {
            leafNodes.push( node );
        }
    } );

    // get the basic info of parent nodes
    leafNodes.forEach( function( node ) {
        var _edge;
        for ( var i = 0; i < edges.length; i++ ) {
            _edge = edges[ i ];
            // if self-loop, omit it.
            if ( _edge.target == _edge.source ) {
                continue;
            }

            if ( _edge.target == node.id ) {
                // parent node's degree > 1
                if ( networkInfo.degree( _edge.source ) > 1 ) {
                    parentNodesHash[ _edge.source ] = parentNodesHash[ _edge.source ]
                        || { id: _edge.source, leaves: [], nonLeaves: [] };
                }
                break;
            }
            else if ( _edge.source == node.id ) {
                if ( networkInfo.degree( _edge.target ) > 1 ) {
                    parentNodesHash[ _edge.target ] = parentNodesHash[ _edge.target ]
                        || { id: _edge.target, leaves: [], nonLeaves: [] };
                }
                break;
            }
        }
    } );

    // complement leaves and nonLeaves arrays for parent node
    for ( var key in parentNodesHash ) {
        ( function( info ) {
            var _p, _neighbors, _neighbor
                , _angleInputs = []
                , _opt
                ;

            _p = networkInfo.nodes( info.id );
            _neighbors = networkInfo.allNeighbors( info.id );
            for ( var i = 0; i < _neighbors.length; i++ ) {
                var _neighbor = _neighbors[ i ];
                // if self-loop, omit it.
                if ( _neighbor.id == info.id ) {
                    continue;
                }

                if ( networkInfo.degree( _neighbor.id ) > 1 ) {
                    info.nonLeaves.push( _neighbor );
                }
                else if ( networkInfo.degree( _neighbor.id ) > 0 ) {
                    info.leaves.push( _neighbor );
                }
            }

            if ( sortLeaves ) {
                info.leaves.sort( function( a, b ) {
                    return sortLeaves.call( null, a, b, _p );
                } );
            }

            // when a totally cluster and the leaves count less than `layoutStarThreshold`
            // , use the star layout
            if ( info.nonLeaves.length == 0
                && info.leaves.length <= layoutStarThreshold ) {
                _layoutStar(
                    _p
                    , info.leaves
                    , opt.layoutStarRadius
                    , sigma.utils.extend( opt )
                );
                return;
            }

            info.nonLeaves.forEach( function( node ) {
                _angleInputs.push( sigma.utils.getAngleInput( node, _p, opt ) );
            } );

            _opt = {
                root: _p
                , angleInputs: _angleInputs
            };

            _opt = sigma.utils.extend( _opt, opt );
            sigma.utils.clustersNodesNew(
                info.leaves
                , _opt
            );

        } )( parentNodesHash[ key ] );
    }

    /**
     * apply star layout on nodes, where root is the central node, leaves is the leaf nodes
     * @param {object} root                         - root node
     * @param {object[]} leaves                     - leaf nodes' array
     * @param {number} radius                       - radius
     * @param {object} options                      - options
     * @param {object} options.readPrefix           - readPrefix
     * @param {object} options.writePrefix          - writePrefix
     */
    function _layoutStar( root, leaves, radius, aptions ) {
        if ( !root || !leaves || radius != +radius
            || leaves.length < 1 ) {
            throw Error( 'clusterLeafNodes: _layoutStar() parameters error.' );
            return;
        }

        var opt = options || {}
            , readPrefix = opt.readPrefix || 'yfh_'
            , writePrefix = opt.writePrefix || 'yfh_'
            , cx = root[ readPrefix + 'x' ] || 0
            , cy = root[ readPrefix + 'y' ] || 0
            , angle = Math.PI / 2
            , angleStep = 2 * Math.PI / leaves.length
            ;

        root[ writePrefix + 'x' ] = cx;
        root[ writePrefix + 'y' ] = cy;
        leaves.forEach( function( _leaf ) {
            _leaf[ writePrefix + 'x' ] = cx + radius * Math.cos( angle );
            _leaf[ writePrefix + 'y' ] = cy + radius * Math.sin( angle );
            angle += angleStep;
        } );
    }

    if ( opt.debug ) {
        return {
            leafNodes: leafNodes
            , parentNodesHash: parentNodesHash
        };
    }

    return me;
};

