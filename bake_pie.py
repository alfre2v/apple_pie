#!/usr/bin/env python3

"""
Simple script to compile and run calculations to stress floating point operations.
Right now we only have the C file: eat_pi.c which computes the value of PI (the mathematical constant)
in a ridiculously slow and inefficient way. Remember, the point is to stress the CPU with floating point operations.
(Although if you want PI with 800 digits just run `eat_pi.bin --help`)
"""
import os
import sys
import logging
import logging.config

from abc import ABC, abstractmethod
import concurrent.futures

from pathlib import Path

import subprocess
import shlex
import time


logger = logging.getLogger(__name__)


def script_path() -> Path:
    return Path(__file__).resolve()


def project_dir() -> Path:
    return script_path().parent


def setup_logging(level='DEBUG', stdout_level='INFO', file_level='DEBUG'):
    log_dir = project_dir() / "logs"
    os.makedirs(log_dir, exist_ok=True)
    logging_config = {
        'version': 1,
        'disable_existing_loggers': False,
        'formatters': {
            'file': {
                'format': '%(asctime)s - %(name)s - %(levelname)s - %(message)s'
            },
            'stdout': {
                'format': '%(asctime)s - %(levelname)s - %(message)s'
            },
        },
        'handlers': {
            'file': {
                'class': 'logging.handlers.RotatingFileHandler',
                'level': file_level,
                'formatter': 'file',
                'filename': log_dir / 'stressor.log',
                'mode': 'a',
                'maxBytes': 1 * 1024 * 1024,  # 1 Mb
                'backupCount': 5,
            },
            'stdout': {
                'class': 'logging.StreamHandler',
                'level': stdout_level,
                'formatter': 'stdout',
                'stream': 'ext://sys.stdout',
            },
        },
        'root': {
            'level': level,
            'handlers': ['file', 'stdout']
        },
    }
    logging.config.dictConfig(logging_config)


class Stressor(ABC):

    _compiler = {
        'c': {
            'gcc': {
                'compile_cmd': 'gcc {source_path} -o {exec_path} {optimizations}',
                'optimizations': '-O3',
            },
        },
    }
    _exec_suffix = '.bin'

    def __init__(self, source_path, lang='C', compiler='gcc'):
        self.lang = lang.lower()
        self.compiler = compiler
        self.source_path = (
            Path(source_path) if Path(source_path).is_file() else project_dir() / source_path
        )
        self.exec_path = self.source_path.with_suffix(self._exec_suffix)
        self.stress_args = []

    def compile(self):
        compile_cmd = self._compiler[self.lang][self.compiler]['compile_cmd'].format(
            source_path=str(self.source_path),
            exec_path=str(self.exec_path),
            optimizations=self._compiler[self.lang][self.compiler]['optimizations']
        )
        logger.info(f'Compiling {self.source_path} with Compile command: {compile_cmd}')
        self.exec_path.unlink(missing_ok=True)
        try:
            out = subprocess.check_output(
                shlex.split(compile_cmd), shell=False, encoding='utf-8', text=True, stderr=subprocess.STDOUT,
            )
        except subprocess.CalledProcessError as e:
            logger.exception(f'return code: {e.returncode}. output: {e.output}. Details: ')
            raise e
        assert self.exec_path.exists()
        if not os.access(self.exec_path, os.X_OK):
            os.chmod(self.exec_path, 0o755)
        logger.info(f'Finished compiling {self.source_path} to {self.exec_path}')

    def stress_arguments(self, *args):
        self.stress_args = list(args[:])

    def _stress(self, capture_output=False):
        run_cmd = [self.exec_path] + [str(arg) for arg in self.stress_args]
        try:
            proc = subprocess.run(
                run_cmd, shell=False, capture_output=capture_output, check=True,
            )
        except subprocess.CalledProcessError as e:
            logger.exception(f'return code: {e.returncode}. output: {e.output}. Details: ')
            raise e
        return proc

    def stress(self, capture_output=True):
        t0 = time.time()
        proc = self._stress(capture_output)
        t1 = time.time()
        if capture_output:
            print(f'Time elapsed: {t1 - t0:.2f} seconds. Output: {proc.stdout.decode("utf-8")}')
        else:
            print(f'Time elapsed: {t1 - t0:.2f} seconds.')
        return (t1 - t0), proc

    def stress_multiprocess(self, parallelism=10):
        with concurrent.futures.ThreadPoolExecutor(max_workers=parallelism) as executor:
            futures = [executor.submit(self.stress, capture_output=True) for _ in range(parallelism)]
            for future in concurrent.futures.as_completed(futures):
                try:
                    t, proc = future.result()
                except Exception as e:
                    logger.exception(f'Exception: {e}. Details: ')
                    raise e
                print(f'Time elapsed: {t:.2f} seconds. Output: {proc.stdout.decode("utf-8")}')
                logger.info(f'Time elapsed: {t:.2f} seconds. Output: {proc.stdout.decode("utf-8")}')


def cli():
    import argparse
    parser = argparse.ArgumentParser(description='Run stress tests on floating point operations.')
    parser.add_argument('-n', type=int, default=10, help='number of repeated stress tests. Default: 10.')
    parser.add_argument('-p', type=int, default=10, help='number of parallel stress tests. Default: 10.')
    return parser.parse_args()


def main():
    setup_logging(level='DEBUG')
    args = cli()
    try:
        logger.info('Starting execution with repeated stress tests (-n): %s, parallel stress tests (-p): %s', args.n, args.p)
        stress_pie = Stressor(source_path='c/eat_pi/eat_pi.c', lang='C', compiler='gcc')
        stress_pie.compile()
        stress_pie.stress_arguments(args.n)
        stress_pie.stress(capture_output=False)
        stress_pie.stress_multiprocess(parallelism=args.p)
    except KeyboardInterrupt:
        print('Used Interrupted execution')
        logger.info('Used Interrupted execution')
    except Exception as e:
        logger.exception(f'Exception: {e}. Details: ')
        raise e
    else:
        logger.info('Execution finished successfully')


if __name__ == '__main__':
    main()

