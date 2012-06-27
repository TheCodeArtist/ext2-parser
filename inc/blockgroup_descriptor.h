// Copyright 2009 Steve Gribble (gribble [at] cs.washington.edu)
// May 22nd, 2009
//
// This file defines the structure of a block group descriptor.
// Every block group (a.k.a. cylinder group from class) has a
// block group descriptor associated with it in the block group
// descriptor table.
//
// Quoting from http://www.nongnu.org/ext2-doc/ext2.html:
//
//// Blocks are clustered into block groups in order to reduce
//// fragmentation and minimise the amount of head seeking when
//// reading a large amount of consecutive data. Information about
//// each block group is kept in the "block group descriptor table"
//// stored in the block(s) immediately after the superblock.  Two
//// blocks near the start of each group are reserved for the block
//// usage bitmap and the inode usage bitmap which show which blocks
//// and inodes are in use. Since each bitmap is limited to a single
//// block, this means that the maximum size of a block group is 8*N
//// blocks, where N is the # bytes in a block.
////
//// The block(s) following the bitmaps in each block group are
//// designated as the inode table for that block group and the
//// remainder are the data blocks. The block allocation algorithm
//// attempts to allocate data blocks in the same block group as the
//// inode which contains them.
//
// As a reminder, since we're using a revision 0 format filesytem
// image in this project, each block group starts with a copy of
// the superblock, and is followed by a copy of the block group
// descriptor table.

#ifndef EXT2READER_INC_BLOCKGROUP_DESCRIPTOR_TABLE_H
#define EXT2READER_INC_BLOCKGROUP_DESCRIPTOR_TABLE_H

#include "types.h"

// A block group descriptor table is an array of block group
// descriptors.
//
// Note that sizeof(os_blockgroup_descriptor_t) == 32 bytes.

struct os_blockgroup_descriptor_t {
  // the 32-bit block ID of the first block of the "block bitmap"
  // for the blockgroup this descriptor represents.
  os_uint32_t bg_block_bitmap;

  // the 32-bit block ID of the first block of the "inode bitmap"
  // for the blockgroup this descriptor represents.
  os_uint32_t bg_inode_bitmap;

  // the 32-bit block ID of the first block of the "inode table"
  // for the blockgroup this descriptor represents.
  os_uint32_t bg_inode_table;

  // the 16bit value indicating the total number of free blocks
  // in the blockgroup this descriptor represents.
  os_uint16_t bg_free_blocks_count;

  // the 16bit value indicating the total number of free inodes
  // in the blockgroup this descriptor represents.
  os_uint16_t bg_free_inodes_count;

  // the 16bit value indicating the number of inodes that have
  // been allocated to directories in the blockgroup this descriptor
  // represents.
  os_uint16_t bg_used_dirs_count;

  // a 16-bit pad value so the structure is 32-bit aligned.
  os_uint16_t bg_pad;

  // 12 bytes of space reserved for future revisions.
  os_uint8_t bg_reserved[12];
};

#endif // EXT2READER_INC_BLOCKGROUP_DESCRIPTOR_TABLE_H
