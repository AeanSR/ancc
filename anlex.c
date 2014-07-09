/*
    anlex: ancc lexer
    Aean, 2014.6.27
*/

#include "ancc.h"

/* == Lexical parser. ============================= */
token_t lexparse() {
    enum {
        S_BREAK,
        S_IDENTIFY,
        S_CONSTNUM,
        S_CONSTNUM_Z,
        S_CONSTFLOAT,
        S_CONSTHEX,
        S_CONSTOCTAT,
        S_STRING,
        S_STRING_ESCAPE,
        S_CHAR,
        S_CHAR_ESCAPE,
        S_COMMENT_STAR,
        S_COMMENT_SLASH,
    };
    int state = S_BREAK;
    int i = 0;
    int lmidx;
    size_t lmlen, len;
    char* head;
    sourceline_t* hcur;

    mclear();
    head = source;
    hcur = cur;
    while( CLA() ) {
        if( CLA() == '\\' && ( CLA( 1 ) == 0x0d || CLA( 1 ) == 0x0a ) ) {
            if ( CLA( 1 ) == 0x0d && CLA( 2 ) == 0x0a )
                CGF( 3 );
            else
                CGF( 2 );
            head = source;
            hcur = cur;
        } else switch( state ) {
            case S_BREAK:
                mclear();
                lmidx = -1;
                lmlen = 0;
                i = 0;
                if( CLA() == '/' && CLA( 1 ) == '/' ) {
                    state = S_COMMENT_SLASH;
                    CGF( 2 );
                    break;
                } else if( CLA() == '/' && CLA( 1 ) == '*' ) {
                    state = S_COMMENT_STAR;
                    CGF( 2 );
                    break;
                } else {
                    while( keylist[i].no != NAL ) {
                        len = strlen( keylist[i].name );
                        if( len > lmlen )
                            if( 0 == strncmp( keylist[i].name, source, len ) ) {
                                lmlen = len;
                                lmidx = i;
                            }
                        i++;
                    }
                    if( lmidx != -1 ) {
                        source += strlen( keylist[lmidx].name );
                        CLA();
                        return token( keylist[lmidx].no, head, hcur, strpool( keylist[lmidx].name ) );
                    }
                }
                if( CLA() == ' ' || CLA() == '\t' || CLA() == '\r' ) {
                    CGF();
                    head = source;
                    hcur = cur;
                } else if( CLA() == '\n' ) {
                    CGF();
                    head = source;
                    hcur = cur;
                } else if( CLA() == '0' ) {
                    state = S_CONSTNUM_Z;
                    mwrite( '0' );
                    CGF();
                } else if( is_digit( CLA() ) ) {
                    state = S_CONSTNUM;
                    mwrite( CGF() );
                } else if( is_alphabet( CLA() ) || CLA() == '_' ) {
                    state = S_IDENTIFY;
                    mwrite( CGF() );
                } else if( CLA() == '"' ) {
                    state = S_STRING;
                    CGF();
                    head = source;
                    hcur = cur;
                } else if( CLA() == '\'' ) {
                    state = S_CHAR;
                    CGF();
                    head = source;
                    hcur = cur;
                } else if( CLA() ) {
                    err( "unexpected token '%c'(0x%02X)", CLA(), ( unsigned )CLA() );
                    CGF();
                }
                break;
            case S_IDENTIFY:
                if( is_alphabet( CLA() ) || is_digit( CLA() ) || CLA() == '_' ) {
                    mwrite( CGF() );
                } else if( CLA() ) {
                    state = S_BREAK;
                    return token( IDENT, head, hcur, strpool( memory ) );
                }
                break;
            case S_CONSTNUM:
                if( is_digit( CLA() ) ) {
                    mwrite( CGF() );
                } else if( CLA() == '.' ) {
                    mwrite( '.' );
                    CGF();
                    state = S_CONSTFLOAT;
                } else if( CLA() ) {
                    if( CLA() == 'U' || CLA() == 'u' )
                        mwrite( CGF() );
                    if( CLA() == 'L' || CLA() == 'l' )
                        mwrite( CGF() );
                    if( CLA() == 'L' || CLA() == 'l' )
                        mwrite( CGF() );
                    state = S_BREAK;
                    return token( C_NUM, head, hcur, strpool( memory ) );
                }
                break;
            case S_CONSTNUM_Z:
                if( is_octaldigit( CLA() ) ) {
                    mwrite( CGF() );
                    state = S_CONSTOCTAT;
                } else if( CLA() == '.' ) {
                    mwrite( '.' );
                    CGF();
                    state = S_CONSTFLOAT;
                } else if( CLA() == 'x' || CLA() == 'X' ) {
                    mwrite( 'x' );
                    CGF();
                    state = S_CONSTHEX;
                } else if( CLA() ) {
                    if( CLA() == 'U' || CLA() == 'u' )
                        mwrite( CGF() );
                    if( CLA() == 'L' || CLA() == 'l' )
                        mwrite( CGF() );
                    if( CLA() == 'L' || CLA() == 'l' )
                        mwrite( CGF() );
                    state = S_BREAK;
                    return token( C_NUM, head, hcur, strpool( memory ) );
                }
                break;
            case S_CONSTOCTAT:
                if( is_octaldigit( CLA() ) ) {
                    mwrite( CGF() );
                } else if( CLA() ) {
                    if( CLA() == 'U' || CLA() == 'u' )
                        mwrite( CGF() );
                    if( CLA() == 'L' || CLA() == 'l' )
                        mwrite( CGF() );
                    if( CLA() == 'L' || CLA() == 'l' )
                        mwrite( CGF() );
                    state = S_BREAK;
                    return token( C_NUM, head, hcur, strpool( memory ) );
                }
                break;
            case S_CONSTFLOAT:
                if( is_digit( CLA() ) ) {
                    mwrite( CGF() );
                } else if( CLA() ) {
                    if( CLA() == 'F' || CLA() == 'f' )
                        mwrite( CGF() );
                    else if( CLA() == 'L' || CLA() == 'l' )
                        mwrite( CGF() );
                    state = S_BREAK;
                    return token( C_NUM, head, hcur, strpool( memory ) );
                }
                break;
            case S_CONSTHEX:
                if( is_hexaldigit( CLA() ) ) {
                    mwrite( CGF() );
                } else if( CLA() ) {
                    if( CLA() == 'U' || CLA() == 'u' )
                        mwrite( CGF() );
                    if( CLA() == 'L' || CLA() == 'l' )
                        mwrite( CGF() );
                    if( CLA() == 'L' || CLA() == 'l' )
                        mwrite( CGF() );
                    state = S_BREAK;
                    return token( C_NUM, head, hcur, strpool( memory ) );
                }
                break;
            case S_STRING:
                if( CLA() == '"' ) {
                    CGF();
                    state = S_BREAK;
                    return token( C_STRING, head, hcur, strpool( memory ) );
                } else if( CLA() == '\\' ) {
                    state = S_STRING_ESCAPE;
                    CGF();
                    mwrite( '\\' );
                } else if( CLA() == '\n' ) {
                    err( "missing terminating \" character" );
                    CGF();
                    state = S_BREAK;
                    return token( C_STRING, head, hcur, strpool( memory ) );
                } else if( CLA() ) {
                    mwrite( CGF() );
                }
                break;
            case S_STRING_ESCAPE:
                if( is_simple_escape( CLA() ) ) {
                    mwrite( CGF() );
                    state = S_STRING;
                } else if( is_octaldigit( CLA() ) ) {
                    mwrite( CGF() );
                    if( is_octaldigit( CLA() ) ) {
                        mwrite( CGF() );
                        if( is_octaldigit( CLA() ) )
                            mwrite( CGF() );
                    }
                    state = S_STRING;
                } else if( CLA() == 'x' && is_hexaldigit( CLA( 1 ) ) ) {
                    mwrite( 'x' );
                    CGF();
                    while( is_hexaldigit( CLA() ) )
                        mwrite( CGF() );
                    state = S_STRING;
                } else if( CLA() ) {
                    warn( "unknown escape sequence: \'\\%c\'", CLA() );
                    mwrite( CGF() );
                    state = S_STRING;
                }
                break;
            case S_CHAR:
                if( CLA() == '\'' ) {
                    if ( strlen( memory ) > 2 || ( strlen( memory ) > 1 && *memory != '\\' ) )
                        warn( "multi-character character constant \"%s\"", memory );
                    else if( *memory == 0 )
                        err( "empty character constant" );
                    state = S_BREAK;
                    CGF();
                    return token( C_CHAR, head, hcur, strpool( memory ) );
                } else if( CLA() == '\\' ) {
                    state = S_CHAR_ESCAPE;
                    CGF();
                    mwrite( '\\' );
                } else if( CLA() == '\n' ) {
                    err( "missing terminating \' character" );
                    CGF();
                    state = S_BREAK;
                    return token( C_CHAR, head, hcur, strpool( memory ) );
                } else if( CLA() ) {
                    mwrite( CGF() );
                }
                break;
            case S_CHAR_ESCAPE:
                if( is_simple_escape( CLA() ) ) {
                    mwrite( CGF() );
                    state = S_CHAR;
                } else if( is_octaldigit( CLA() ) ) {
                    mwrite( CGF() );
                    if( is_octaldigit( CLA() ) ) {
                        mwrite( CGF() );
                        if( is_octaldigit( CLA() ) )
                            mwrite( CGF() );
                    }
                    state = S_CHAR;
                } else if( CLA() == 'x' && is_hexaldigit( CLA( 1 ) ) ) {
                    mwrite( 'x' );
                    CGF();
                    while( is_hexaldigit( CLA() ) )
                        mwrite( CGF() );
                    state = S_CHAR;
                } else if( CLA() ) {
                    warn( "unknown escape sequence: \'\\%c\'", CLA() );
                    mwrite( CGF() );
                    state = S_CHAR;
                }
                break;
            case S_COMMENT_SLASH:
                if( CLA() )
                    if ( CGF() == '\n' ) {
                        state = S_BREAK;
                    }
                break;
            case S_COMMENT_STAR:
                if ( CLA() == '*' && CLA( 1 ) == '/' ) {
                    state = S_BREAK;
                    CGF( 2 );
                } else if( CLA() ) {
                    CGF();
                }
                break;
            default:
                printf( "internal error: unexpected state %d of ancc lexer\n", state );
                exit( -1 );
                break;
            }
    }
    return token( NAL, 0, 0, 0 );
}
