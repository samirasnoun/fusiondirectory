<?php
/**
 * FDStandard_Sniffs_WhiteSpace_FunctionCallArgumentSpacingSniff.
 *
 * Checks that calls to methods and functions are spaced correctly.
 *
 * Modified version of PHP_CodeSniffer Generic_Sniffs_Function_FunctionCallArgumentSpacingSniff
 * by Greg Sherwood <gsherwood@squiz.net> and Marc McIntyre <mmcintyre@squiz.net>
 *
 * @author    Côme Bernigaud <come.bernigaud@laposte.net>
 * @license   http://matrix.squiz.net/developer/tools/php_cs/licence BSD Licence
 */

class FDStandard_Sniffs_WhiteSpace_FunctionCallArgumentSpacingSniff implements PHP_CodeSniffer_Sniff
{
    /**
     * Do we want exactly one space between function args.
     *
     * If TRUE, we want exactly one space after comma between function args. If FALSE,
     * we want at least 1 space (but can be more).
     *
     * @var bool
     */
    public $exact = false;

    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array
     */
    public function register()
    {
        return array(T_STRING);

    }//end register()


    /**
     * Processes this test, when one of its tokens is encountered.
     *
     * @param PHP_CodeSniffer_File $phpcsFile The file being scanned.
     * @param int                  $stackPtr  The position of the current token in the
     *                                        stack passed in $tokens.
     *
     * @return void
     */
    public function process(PHP_CodeSniffer_File $phpcsFile, $stackPtr)
    {
        $tokens = $phpcsFile->getTokens();

        // Skip tokens that are the names of functions or classes
        // within their definitions. For example:
        // function myFunction...
        // "myFunction" is T_STRING but we should skip because it is not a
        // function or method *call*.
        $functionName    = $stackPtr;
        $ignoreTokens    = PHP_CodeSniffer_Tokens::$emptyTokens;
        $ignoreTokens[]  = T_BITWISE_AND;
        $functionKeyword = $phpcsFile->findPrevious($ignoreTokens, ($stackPtr - 1), null, true);
        if ($tokens[$functionKeyword]['code'] === T_FUNCTION) {
            $whatisit = "function declaration";
        } elseif ($tokens[$functionKeyword]['code'] === T_CLASS) {
            return;
        } else {
            $whatisit = "function call";
        }

        // If the next non-whitespace token after the function or method call
        // is not an opening parenthesis then it cant really be a *call*.
        $openBracket = $phpcsFile->findNext(PHP_CodeSniffer_Tokens::$emptyTokens, ($functionName + 1), null, true);
        if ($tokens[$openBracket]['code'] !== T_OPEN_PARENTHESIS) {
            return;
        }

        $closeBracket = $tokens[$openBracket]['parenthesis_closer'];

        $nextSeperator = $openBracket;
        while (($nextSeperator = $phpcsFile->findNext(array(T_COMMA, T_VARIABLE), ($nextSeperator + 1), $closeBracket)) !== false) {
            // Make sure the comma or variable belongs directly to this function call,
            // and is not inside a nested function call or array.
            $brackets    = $tokens[$nextSeperator]['nested_parenthesis'];
            $lastBracket = array_pop($brackets);
            if ($lastBracket !== $closeBracket) {
                continue;
            }

            if ($tokens[$nextSeperator]['code'] === T_COMMA) {
                if ($tokens[($nextSeperator - 1)]['code'] === T_WHITESPACE) {
                    $error = 'Space found before comma in '.$whatisit;
                    $phpcsFile->addError($error, $stackPtr, 'SpaceBeforeComma');
                }

                if ($tokens[($nextSeperator + 1)]['code'] !== T_WHITESPACE) {
                    $error = 'No space found after comma in '.$whatisit;
                    $phpcsFile->addError($error, $stackPtr, 'NoSpaceAfterComma');
                } else {
                    // If there is a newline in the space, then the must be formatting
                    // each argument on a newline, which is valid, so ignore it.
                    if (strpos($tokens[($nextSeperator + 1)]['content'], $phpcsFile->eolChar) === false) {
                        $space = strlen($tokens[($nextSeperator + 1)]['content']);
                        if (($space > 1) && ($this->exact)) {
                            $error = 'Expected 1 space after comma in '.$whatisit.'; %s found';
                            $data  = array($space);
                            $phpcsFile->addError($error, $stackPtr, 'TooMuchSpaceAfterComma', $data);
                        }
                    }
                }
            } else {
                // Token is a variable.
                $nextToken = $phpcsFile->findNext(PHP_CodeSniffer_Tokens::$emptyTokens, ($nextSeperator + 1), $closeBracket, true);
                if ($nextToken !== false) {
                    if ($tokens[$nextToken]['code'] === T_EQUAL) {
                        if (($tokens[($nextToken - 1)]['code']) !== T_WHITESPACE) {
                            $error = 'Expected 1 space before = sign of default value';
                            $phpcsFile->addError($error, $stackPtr, 'NoSpaceBeforeEquals');
                        }

                        if ($tokens[($nextToken + 1)]['code'] !== T_WHITESPACE) {
                            $error = 'Expected 1 space after = sign of default value';
                            $phpcsFile->addError($error, $stackPtr, 'NoSpaceAfterEquals');
                        }
                    }
                }
            }//end if
        }//end while

    }//end process()


}//end class

?>
