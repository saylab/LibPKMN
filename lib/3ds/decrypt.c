#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <save.h>
#include <utils.h>
#include <sys/stat.h>

#ifdef _MSC_VER
#include <direct.h>
#else
#include <unistd.h>
#endif

#define XORPAD_SIZE	512
#define SECTOR_SIZE	0x1000

#define DEBUG

#ifdef _MSC_VER
    #define snprintf _snprintf
#endif

int do_wearlevel = 0;
int do_decrypt = 0;
int do_filesys = 0;

size_t file_size(FILE* fp){
    long tmp, ret;

	tmp = ftell(fp);
	fseek(fp, 0, SEEK_END);
	ret = ftell(fp);
	fseek(fp, tmp, SEEK_SET);
	return ret;
}

void decrypt(void* buf, size_t size, void* xorpad){
    int i;

	for(i = 0; i < size; i++)
		((uint8_t*)buf)[i] ^= ((uint8_t*)xorpad)[i % XORPAD_SIZE];		
}

int wearlevel(void* buffer, size_t size, void* dest){
    uint8_t* buf;
    struct sector_entry* entries;
    struct long_sector_entry* sectors;
    struct header_entry *header;
    int num_entries, i, j, sec;
    char filename[256];
    FILE* out;

	buf = (uint8_t*)buffer;
	sectors = (struct long_sector_entry*)buffer;

	// Find the start
	num_entries = (size / SECTOR_SIZE) - 1;
	entries = calloc(num_entries, sizeof(*entries));
	sectors = (struct long_sector_entry*)(buf + ((num_entries + 1) * sizeof(*entries)));

	// Populate the base state
	header = (struct header_entry*)buf;
	for(i = 0; i < num_entries; i++){
		entries[i].phys_sec = header[i].phys_sec;
		entries[i].virt_sec = i;
		memcpy(entries[i].chksums, header[i].chksums, sizeof(entries[0].chksums));
	}

	// Apply the journal
	while(sectors->magic == SECTOR_MAGIC){
		sec = sectors->sector.virt_sec;
		entries[sec] = sectors->sector;
		entries[sec].phys_sec |= 0x80;
		sectors++;
	}

#ifdef DEBUG
	for(i = 0; i < num_entries; i++){
		printf("virt: %d, phys: %d, cnt: %d,%s", 
				entries[i].virt_sec,
				entries[i].phys_sec & 0x7F,
				entries[i].virt_realloc_cnt,
				entries[i].phys_sec & 0x80 ? " (in use) " : " ");
		printf("chksums: ");
		for(j = 0; j < sizeof(entries[i].chksums); j++)
			printf("%02X ", entries[i].chksums[j]);
		printf("\n");
	}
#endif /* DEBUG */

	for(i = 0; i < num_entries; i++){
		if((entries[i].phys_sec & 0x7F) * SECTOR_SIZE < size){
			memcpy( &((uint8_t*)dest)[SECTOR_SIZE * i],
				&buf[SECTOR_SIZE * (entries[i].phys_sec & 0x7F)],
				SECTOR_SIZE);
		} else {
			fprintf(stderr, "Illegal physical sector (%d) in blockmap (%d)\n", entries[i].phys_sec, i);
		}
#ifdef SEGHER		
		for(j = 0; j < 8; j++){
			if(entries[i].chksums[j]){
				snprintf(filename, sizeof(filename), "block_%02X_%02X%02X%02X%02X)",
				entries[i].chksums[j],
				buf[(SECTOR_SIZE * entries[i].phys_sec) + j * 0x200],
				buf[(SECTOR_SIZE * entries[i].phys_sec) + j * 0x200 + 1],
				buf[(SECTOR_SIZE * entries[i].phys_sec) + j * 0x200 + 2],
				buf[(SECTOR_SIZE * entries[i].phys_sec) + j * 0x200 + 3]);
			
				out = fopen(filename, "wb");
				fwrite(&buf[(SECTOR_SIZE * (entries[i].phys_sec & 0x7F)) + j * 0x200], 1, 0x200, out);
				fclose(out);
			}
		}
#endif

	}
	return 0;
}

void parse_fs(void*, char*);

#define PART_BASE		0x2000
#define HASH_TBL_LEN_OFF	0x9C
#define PARTITION_LEN_OFF	0xA4
#define FS_INDEX_OFF		0x39
#define DIFI_LENGTH		0x130

void parse_partitions(uint8_t* buf, char* target_dir){
	char part_dir[256];
	uint8_t hash[SHA256_DIGEST_LENGTH];
    uint8_t* buffer;
    uint32_t cur_part, part_length, hash_tbl_length;
    int i, j, x;

	buffer = buf + 0x200;
	cur_part = PART_BASE;
	part_length = 0;
	hash_tbl_length = 0;

	if(mkdir(target_dir, 0766) < 0){
		fprintf(stderr, "Couldn't create dir %s\n", target_dir);
		return;
	}

	chdir(target_dir);
	while(!strncmp((char*)buffer, "DIFI", 4)){
		snprintf(part_dir, sizeof(part_dir), "part-%d", buffer[FS_INDEX_OFF]);
		hash_tbl_length = *((uint32_t*)&buffer[HASH_TBL_LEN_OFF]);
		part_length = *((uint32_t*)&buffer[PARTITION_LEN_OFF]);
		for(i = 0; i < hash_tbl_length; i += SHA256_DIGEST_LENGTH){
			for(j = 0; j < part_length; j += SECTOR_SIZE){
				sha256(buf + j + cur_part + hash_tbl_length, (part_length - j > SECTOR_SIZE) ? SECTOR_SIZE : part_length - j, hash);
				if(!memcmp(buf + i + cur_part, hash, sizeof(hash))){
					printf("Block (%08X) matches entry %d (", j + cur_part + hash_tbl_length, i / SHA256_DIGEST_LENGTH);
					for(x = 0; x < SHA256_DIGEST_LENGTH; x++)
						printf("%02x", hash[x]);
					printf(")\n");
				}
			}
		}
#ifdef DEBUG
		printf("Partition @ %06X (%06X)\n", cur_part, part_length + hash_tbl_length);
#endif /* DEBUG */
		cur_part += hash_tbl_length;
		parse_fs(buf + cur_part, part_dir);
		cur_part += part_length;
		buffer += DIFI_LENGTH;
	}	
}

#define	FST_OFF_OFFSET	(0x6C)
#define FST_BASE_OFFSET (0x58)

#define FS_BLOCK_SIZE	0x200

void parse_fs(void* buf, char* target_dir){
    uint8_t* buffer;
    struct fs_entry* entries;
    int nr_entries;
    uint16_t fst_base;
    uint32_t fst_offset;
    int i;
    FILE* fp;
    char filename[256];

	buffer = (uint8_t*)buf;
	nr_entries = 0;

	if(strncmp((char*)buffer, "SAVE", 4)){
		fprintf(stderr, "Failed to find filesystem.\n");
		return;
	}
	
	fst_base = *((uint16_t*)&buffer[FST_BASE_OFFSET]);
	fst_offset = (*((uint32_t*)&buffer[FST_OFF_OFFSET])) * FS_BLOCK_SIZE;
	entries = (struct fs_entry*)(&buffer[fst_offset + fst_base]);
	nr_entries = entries->node_cnt;

	if(nr_entries < 2){
		fprintf(stderr, "Filesystem contains no file nodes.\n");
		return;
	}

	// Skip the root node
	entries++;
	nr_entries--;

#ifdef DEBUG
	for(i = 0; i < nr_entries; i++){
		if(entries[i].magic != FILE_MAGIC){
			fprintf(stderr, "fs_entry %d did not have file magic.\n", i);
			return;
		}
		
		fprintf(stderr, "File: %s, size: %u bytes, block_nr: %u\n",
			entries[i].filename,
			(unsigned int)entries[i].size,
			entries[i].block_nr);
	}
#endif /* DEBUG */
	if(mkdir(target_dir, 0766) < 0){
		fprintf(stderr, "Couldn't create dir %s\n", target_dir);
		return;
	}

	for(i = 0; i < nr_entries; i++){
		if(entries[i].magic != FILE_MAGIC){
			fprintf(stderr, "fs_entry %d did not have file magic.\n", i);
			break;
		}

		snprintf(filename, sizeof(filename), "%s/%s", target_dir, entries[i].filename);
		fp = fopen(filename, "wb");
		if(!fp){
			fprintf(stderr, "Failed to open %s\n", filename);
			continue;
		}

		fwrite(&buffer[fst_base + (entries[i].block_nr * FS_BLOCK_SIZE)],
			1,
			entries[i].size,
			fp);
		fclose(fp);
	}	
}