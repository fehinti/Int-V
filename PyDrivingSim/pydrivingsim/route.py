# Authors : Victor Fajemirokun
# Date    : 15/01/2025
# License : MIT

import pygame
import random

from pydrivingsim import VirtualObject, World

class RouteSprite(pygame.sprite.Sprite):
    def __init__(self, point):
        super().__init__()
        self.point = point
        # self.rect = pygame.draw.circle(World().screen, (255, 0, 0), (self.point.x, self.point.y), 10)
        # print(self.rect)
        # w, h = sprite.get_size()
        # self.image_fix.append(pygame.transform.smoothscale(sprite, (2, 2)))

        # self.image = self.image_fix[0]
        # self.rect = self.image_fix[0].get_rect()
        # self.size = self.image_fix[0].get_size()

    def draw(self):
        center = [
            (self.point.x - World().get_world_pos()[0]) * World().scaling_factor + World().screen_world_center[0],
            (World().get_world_pos()[1] - self.point.y) * World().scaling_factor + World().screen_world_center[1]
        ]
        self.rect = pygame.draw.circle(World().screen, self.point.colour, center, self.point.size)

    def update(self) -> None:
        self.rect.center = [
            (self.point.x - World().get_world_pos()[0]) * World().scaling_factor + World().screen_world_center[0],
            (World().get_world_pos()[1] - self.point.y) * World().scaling_factor + World().screen_world_center[1]
        ]
        # self.image = self.image_fix[self.route.state]

class Route(VirtualObject):
    __metadata = {
        "dt": 0.1
    }
    RED = (255, 0, 0)
    BLUE = (0, 0, 255)
    PT_SIZE = 1
    PATH_PT_SIZE = 2
    def __init__( self, x, y ):
        super().__init__(self.__metadata["dt"])
        self.state = 0
        self.x = x
        self.y = y
        self.colour = self.BLUE
        self.size = self.PT_SIZE
        self.reset()

        # Sprite
        self.sprite = PointSprite(self)
        # self.group = pygame.sprite.Group()
        # self.group.add(self.sprite)        

    def set_point(self, x, y):
        self.x = x
        self.y = y
        self.sprite.draw()

    def set_color(self, colour):
        if colour == "RED":
            self.colour = self.RED
            self.size = self.PATH_PT_SIZE
        elif colour == "BLUE":
            self.colour = self.BLUE
            self.size = self.PT_SIZE
        else:
            print("Invalid colour")

    def reset( self ):
        self.state = 0

    def render( self ):
        self.sprite.draw()
        # self.sprite.update()
        # self.group.draw(World().screen)

