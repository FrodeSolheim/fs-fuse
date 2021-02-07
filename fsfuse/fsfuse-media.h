#ifndef FSFUSE_MEDIA_H_
#define FSFUSE_MEDIA_H_

void fsfuse_media_init(void);

void fsfuse_media_on_tape_open(const char *path);
void fsfuse_media_on_tape_close(void);

#endif  // FSFUSE_MEDIA_H_
