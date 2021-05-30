'use strict'

import fsExtra from 'fs-extra'
import archiver from 'archiver'
import octokitRequest from '@octokit/request'

const { ensureDirSync, createWriteStream, copy, remove, readFile } = fsExtra
const { request } = octokitRequest

const version = process.argv[2]
const accessToken = process.argv[3]

async function bundleCode() {
  ensureDirSync(`libs/texture-packer-${version}/`)
  return copy('texture-packer/Release/', `libs/texture-packer-${version}/`, { recursive: true, filter: (src, dest) => {
    return src.endsWith('/') || src.endsWith('.dll') || src.endsWith('.exe')
  } })
}

async function bundleResources() {
  ensureDirSync(`libs/texture-packer-${version}/res/main/`)
  return copy('texture-packer/res/main/', `libs/texture-packer-${version}/res/main/`, { recursive: true })
}

async function bundle() {
  return Promise.all([
    bundleCode(),
    bundleResources()
  ])
}

async function zip() {
  const archive = archiver('zip', { zlib: { level: 9 } })
  archive.on('warning', err => {
    if (err.code == 'ENOENT') console.log(err)
    else throw err
  }).on('error', err => {
    throw err
  }).pipe(createWriteStream(`libs/texture-packer-${version}.zip`))
  return archive.directory(`libs/texture-packer-${version}/`, `texture-packer-${version}/`).finalize()
}

async function publish() {
  const result = await request('POST /repos/{owner}/{repo}/releases', {
    headers: {
      authorization: `token ${accessToken}`
    },
    owner: 'ii887522',
    repo: 'texture-packer',
    tag_name: `v${version}`,
    name: `${version}`
  })
  return request('POST /repos/{owner}/{repo}/releases/{release_id}/assets{?name,label}', {
    headers: {
      authorization: `token ${accessToken}`,
      'content-type': 'application/zip'
    },
    baseUrl: 'https://uploads.github.com',
    owner: 'ii887522',
    repo: 'texture-packer',
    release_id: result.data.id,
    name: `texture-packer-${version}.zip`,
    data: await readFile(`libs/texture-packer-${version}.zip`)
  })
}

function clean() {
  remove(`libs/texture-packer-${version}`)
  remove(`libs/texture-packer-${version}.zip`)
}

(async () => {
  await bundle()
  await zip()
  await publish()
  clean()
})()
