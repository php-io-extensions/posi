<?php

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

class RecvFileOptimizer extends OptimizerAbstract
{
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            throw new CompilerException("'recv_file' requires two or three parameters", $expression);
        }

        $paramCount = count($expression['parameters']);
        if ($paramCount < 2 || $paramCount > 3) {
            throw new CompilerException("'recv_file' requires two or three parameters", $expression);
        }

        [$symbolVariable, $resolvedParams, $symbol] = $this->processStringOptimizer(
            $call,
            $context,
            $expression
        );

        $context->headersManager->add('src/system-api');

        $flags = $paramCount === 3 ? $resolvedParams[2] : '0';

        $context->codePrinter->output(
            'posix_recv(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ', ' . $flags . ');'
        );

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
