import argparse
import sys

import h5py
import numpy as np


CACHE_SIZE = 256 * 1024 * 1024
CHUNK_SIZE = 100


def main(*, srcfile, outfile):
    src = h5py.File(srcfile, "r", rdcc_nbytes=CACHE_SIZE)
    out = h5py.File(outfile, "w")

    genes = src["genes"][:]
    expressions = src["expressions"]
    n_genes, n_cells = expressions.shape

    out["genes"] = genes
    corr_dataset = out.create_dataset(
        "correlations",
        shape=(n_genes, n_genes), dtype=np.float32, shuffle=True, compression=1,
    )

    for beg in range(0, n_genes, CHUNK_SIZE):
        end = min(beg + CHUNK_SIZE, n_genes)

        X = standardize(expressions[beg:end])
        c = np.empty(X.shape[0])

        for i in range(n_genes):
            y = standardize(expressions[i, :])
            r = X @ y / n_cells
            corr_dataset[beg:end, i] = r


def standardize(x, epsilon=1e-6):
    u = np.log(1 + x)
    m = np.mean(u, axis=-1, keepdims=True)
    s = np.std(u, axis=-1, keepdims=True)
    return (u - m) / (s + epsilon)


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("srcfile", type=str)
    parser.add_argument("outfile", type=str)
    return vars(parser.parse_args())


main(**parse_args())
