<?php

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

class OpenFileOptimizer extends OptimizerAbstract
{
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            throw new CompilerException("'open_file' requires at least 2 parameters. No arguments set.", $expression);
        }

        $paramCount = count($expression['parameters']);

        if ($paramCount == 1) {
            throw new CompilerException("'open_file' requires at least 2 parameters. 2nd argument missing.", $expression);
        }

        if ($paramCount > 3) {
            throw new CompilerException("'open_file' accepts at most 3 parameters.", $expression);
        }

        /**
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable();
        if (!$symbolVariable->isInt()) {
            throw new CompilerException("File Descriptors only can be stored in int variables", $expression);
        }

        $context->headersManager->add('src/system-api');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        $modeParam = $paramCount === 3 ? $resolvedParams[2] : '0644';

        return new CompiledExpression(
            'int',
            "posix_open({$resolvedParams[0]}, {$resolvedParams[1]}, {$modeParam})",
            $expression
        );
    }
}
