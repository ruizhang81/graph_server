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
